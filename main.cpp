#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct Point
{
    double x, y;    // x, y
    int cluster;    // 중심점
    double minDist; // 가까운 중심점으로부터의 최단 거리

    Point() : x(0.0), y(0.0), cluster(-1), minDist(__DBL_MAX__) {}
    Point(double _x, double _y) : x(_x), y(_y), cluster(-1), minDist(__DBL_MAX__) {}

    double getDistance(Point p) {
        return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
    }
};

// 파일에서부터 데이터를 읽어올 경우 사용
vector<Point> readCsv()
{
    vector<Point> points;
    string input;
    ifstream file("data.csv");

    while (getline(file, input)) {
        stringstream str(input);
        string bit;
        double x, y;

        // 자르기
        getline(str, bit, ',');
        x = stof(bit);
        getline(str, bit, '\n');
        y = stof(bit);

        points.push_back(Point(x, y));
    }

    return points;
}

float util_RandFloat(float a, float b)
{
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

// 랜덤으로 데이터를 만들 경우 사용
vector<Point> makeRandomData(int count)
{
    vector<Point> points;

    for (int i = 0; i < count; ++i)
    {
        // 원하는 범위를 설정해 주면 된다. 기본값 (0 ~ 200)
        points.push_back(Point(util_RandFloat(0, 200), util_RandFloat(0, 200)));
    }

    return points;
}

void makeDataCsv(vector<Point>* data) 
{
    ofstream file;
    file.open("data.csv");

    for (auto it = data->begin(); it != data->end(); ++it) 
    {
        file << it->x << "," << it->y << endl;
    }

    file.close();
}

void makeOutputCsv(vector<Point>* data) 
{
    ofstream file;
    file.open("output.csv");
    file << "x,y,c" << endl;

    for (auto it = data->begin(); it != data->end(); ++it) 
    {
        file << it->x << "," << it->y << "," << it->cluster << endl;
    }

    file.close();
}

/**
 * @brief k-means 알고리즘
 * 
 * @param points 데이터
 * @param iterCount 알고리즘 반복 횟수
 * @param centerCount 중심점의 개수
 */
void kMeansClustering(vector<Point>* points, int iterCount, int centerCount)
{
    int n = points->size();

    // 랜덤으로 중심점을 설정한다. (index를 저장)
    vector<Point> centroids;
    for (int i = 0; i < centerCount; ++i)
    {
        centroids.push_back(points->at(rand() % n));
    }

    // 지정한 횟수만큼 k-means 알고리즘을 반복한다.
    for (int i = 0; i < iterCount; ++i)
    {
        // 중심점을 기준으로 계산한다.
        for (auto cent = begin(centroids); cent != end(centroids); ++cent)
        {
            // 현재 cent의 index를 가져온다.
            int clusterId = cent - begin(centroids);

            // points vector를 돌면서 중심점 기준으로 거리를 계산한다.
            for (auto it = points->begin(); it != points->end(); ++it)
            {
                auto p = *it;
                auto dist = cent->getDistance(p);

                // minDist보다 짧으면 저장하고, minDist를 갱신한다.
                if (dist < p.minDist)
                {
                    p.minDist = dist;
                    p.cluster = clusterId;
                }

                *it = p;
            }
        }

        // 실제 의미 있는 중심점을 할당한다. => 평균 좌표를 구한다.
        vector<int> counters;
        vector<double> sumX, sumY;

        counters.resize(centerCount);
        sumX.resize(centerCount);
        sumY.resize(centerCount);

        for (auto& it : *points)
        { 
            int clusterId = it.cluster;

            counters[clusterId]++;
            sumX[clusterId] += it.x;
            sumY[clusterId] += it.y;

            // 랜덤 중심점에서 계산해 놨던 거리를 모두 리셋한다.
            it.minDist = __DBL_MAX__;
        }

        // 실제 중심점을 계산한다.
        for (auto c = begin(centroids); c != end(centroids); ++c) 
        {
            int clusterId = c - begin(centroids);
            c->x = sumX[clusterId] / counters[clusterId];
            c->y = sumY[clusterId] / counters[clusterId];
        }
    }
}

int main()
{
    srand(time(NULL));

    int dataCount = 500;

    //vector<Point> points = readCsv();

    vector<Point> points = makeRandomData(dataCount);
    makeDataCsv(&points);

    kMeansClustering(&points, 1000, 5);
    makeOutputCsv(&points);

    return 0;
}