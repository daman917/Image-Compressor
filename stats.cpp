#include "stats.h"

stats::stats(PNG & im){

/* Your code here!! */
    int w = im.width();
    int h = im.height();

    setArray(sumRed, w, h);
    setArray(sumGreen, w, h);
    setArray(sumBlue, w, h);

    setArray(sumsqRed, w, h);
    setArray(sumsqGreen, w, h);
    setArray(sumsqBlue, w, h);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            sumRed[i][j] += im.getPixel(i, j)->r;
            sumGreen[i][j] += im.getPixel(i, j)->g;
            sumBlue[i][j] += im.getPixel(i, j)->b;

            sumsqRed[i][j] += pow(im.getPixel(i, j)->r, 2);
            sumsqGreen[i][j] += pow(im.getPixel(i, j)->g, 2);
            sumsqBlue[i][j] += pow(im.getPixel(i, j)->b, 2);

            if (i == 0) {
                if (j != 0) {
                    sumRed[i][j] += sumRed[i][j-1];
                    sumGreen[i][j] += sumGreen[i][j-1];
                    sumBlue[i][j] += sumBlue[i][j-1];

                    sumsqRed[i][j] += sumsqRed[i][j-1];
                    sumsqGreen[i][j] += sumsqGreen[i][j-1];
                    sumsqBlue[i][j] += sumsqBlue[i][j-1];
                }
            }

            if (j == 0) {
                if (i != 0) {
                    sumRed[i][j] += sumRed[i-1][j];
                    sumGreen[i][j] += sumGreen[i-1][j];
                    sumBlue[i][j] += sumBlue[i-1][j];

                    sumsqRed[i][j] += sumsqRed[i-1][j];
                    sumsqGreen[i][j] += sumsqGreen[i-1][j];
                    sumsqBlue[i][j] += sumsqBlue[i-1][j];
                }
            }

            if (i != 0 && j != 0) {
                sumRed[i][j] += sumRed[i-1][j] + sumRed[i][j-1] - sumRed[i-1][j-1];
                sumGreen[i][j] += sumGreen[i-1][j] + sumGreen[i][j-1] - sumGreen[i-1][j-1];
                sumBlue[i][j] += sumBlue[i-1][j] + sumBlue[i][j-1] - sumBlue[i-1][j-1];

                sumsqRed[i][j] += sumsqRed[i-1][j] + sumsqRed[i][j-1] - sumsqRed[i-1][j-1];
                sumsqGreen[i][j] += sumsqGreen[i-1][j] + sumsqGreen[i][j-1] - sumsqGreen[i-1][j-1];
                sumsqBlue[i][j] += sumsqBlue[i-1][j] + sumsqBlue[i][j-1] - sumsqBlue[i-1][j-1];
            }
        }
    }

}

void stats::setArray(vector< vector< long >> & array, int w, int h) {
    array.resize(w, vector<long>(h, 0));
}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */

    switch (channel) {
        case 'r':
            return getSumValue(sumRed, ul, w, h);
            break;
        case 'g':
            return getSumValue(sumGreen, ul, w, h);
            break;
        case 'b':
            return getSumValue(sumBlue, ul, w, h);
            break;
    }
    return 0;
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */

    switch (channel) {
        case 'r':
            return getSumValue(sumsqRed, ul, w, h);
            break;
        case 'g':
            return getSumValue(sumsqGreen, ul, w, h);
            break;
        case 'b':
            return getSumValue(sumsqBlue, ul, w, h);
            break;
    }
    return 0;
}

long stats::getSumValue(vector< vector< long >> & sumChannel, pair<int,int> ul, int w, int h) {

    if (ul.first == 0) {
        if (ul.second != 0) {
            return sumChannel[ul.first+w-1][ul.second+h-1] - sumChannel[ul.first+w-1][ul.second-1];
        }
    }

    if (ul.second == 0) {
        if (ul.first != 0) {
            return sumChannel[ul.first+w-1][ul.second+h-1] - sumChannel[ul.first-1][ul.second+h-1];
        }
    }

    if (ul.first != 0 && ul.second != 0) {
        return sumChannel[ul.first+w-1][ul.second+h-1] - sumChannel[ul.first-1][ul.second+h-1] - sumChannel[ul.first+w-1][ul.second-1] + sumChannel[ul.first-1][ul.second-1];
    }

    return sumChannel[ul.first+w-1][ul.second+h-1];
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
/* Your code here!! */

    if (w==0 || h==0) {
        return 0;
    }

    double rVar = getSumSq('r', ul, w, h) - pow(getSum('r', ul, w, h), 2) / (w*h);
    double gVar = getSumSq('g', ul, w, h) - pow(getSum('g', ul, w, h), 2) / (w*h);
    double bVar = getSumSq('b', ul, w, h) - pow(getSum('b', ul, w, h), 2) / (w*h);

    return rVar + gVar + bVar;

}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
/* Your code here!! */

    int rAvg = (int) getSum('r', ul, w, h) / (w*h);
    int gAvg = (int) getSum('g', ul, w, h) / (w*h);
    int bAvg = (int) getSum('b', ul, w, h) / (w*h);

    return RGBAPixel(rAvg, gAvg, bAvg);

}