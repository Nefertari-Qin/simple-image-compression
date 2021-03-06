#define CATCH_CONFIG_MAIN
#include "cs221util/catch.hpp"
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "stats.h"
#include "quadtree.h"
#include "qtcount.h"
#include "qtvar.h"

using namespace std;
using namespace cs221util;


/*
TEST_CASE("qtcount::basic canada","[weight=1][part=qtcount]"){
    PNG img;
    img.readFromFile("../spec/PA3pic5.png");
    
    qtcount t1(img); 
    t1.prune(4000);
    PNG result = t1.render();

    result.writeToFile("../spec/PA3pic6.png");
} 
*/

TEST_CASE("stats::basic rectArea","[weight=1][part=stats]"){

    PNG data; data.resize(2,2);
    stats s(data);
    long result = s.rectArea(1);

    REQUIRE(result == 4);
    
}

TEST_CASE("stats::basic getAvg","[weight=1][part=stats]"){
    PNG data; data.resize(2,2);
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 2; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = 20 * i + 3 * j;
            p->g = 3 * i + 20 * j;
            p->b = 23 * i + 23 * j;
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(0,0);
    RGBAPixel result = s.getAvg(ul,1);
    RGBAPixel expected(11,11,23);

    REQUIRE(result == expected);
}

TEST_CASE("stats::basic getAvg self_made square5x5(0,0)","[weight=1][part=stats]"){
    PNG data; data.resize(5,5);
    for (int i = 0; i < 5; i ++){
        for (int j = 0; j < 5; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = i + j + 1;
            p->g = 10 * (i + j + 1);
            p->b = 20 * (i + j + 1);
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(0,0);
    RGBAPixel result = s.getAvg(ul,2);
    RGBAPixel expected(4,40,80);

    REQUIRE(result == expected);
}

TEST_CASE("stats::basic getAvg self_made square5x5(1,1)","[weight=1][part=stats]"){
    PNG data; data.resize(5,5);
    for (int i = 0; i < 5; i ++){
        for (int j = 0; j < 5; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = i + j + 1;
            p->g = 10 * (i + j + 1);
            p->b = 20 * (i + j + 1);
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(1,1);
    RGBAPixel result = s.getAvg(ul,2);
    RGBAPixel expected(6,60,120);

    REQUIRE(result == expected);
}

TEST_CASE("stats::basic getAvg self_made square5x5(1,0)","[weight=1][part=stats]"){
    PNG data; data.resize(5,5);
    for (int i = 0; i < 5; i ++){
        for (int j = 0; j < 5; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = i + j + 1;
            p->g = 10 * (i + j + 1);
            p->b = 20 * (i + j + 1);
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(1,0);
    RGBAPixel result = s.getAvg(ul,2);
    RGBAPixel expected(5,50,100);

    REQUIRE(result == expected);
}


TEST_CASE("stats::basic getAvg self_made rectangle4x5(1,0)","[weight=1][part=stats]"){
    PNG data; data.resize(5,4);
    for (int i = 0; i < 5; i ++){
        for (int j = 0; j < 4; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = i + j + 1;
            p->g = 10 * (i + j + 1);
            p->b = 20 * (i + j + 1);
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(1,0);
    RGBAPixel result = s.getAvg(ul,2);
    RGBAPixel expected(5,50,100);

    REQUIRE(result == expected);
}

TEST_CASE("stats::basic getAvg self_made rectangle4x5(0,0)","[weight=1][part=stats]"){
    PNG data; data.resize(5,4);
    for (int i = 0; i < 5; i ++){
        for (int j = 0; j < 4; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = i + j + 1;
            p->g = 10 * (i + j + 1);
            p->b = 20 * (i + j + 1);
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(0,0);
    RGBAPixel result = s.getAvg(ul,2);
    RGBAPixel expected(4,40,80);

    REQUIRE(result == expected);
}

TEST_CASE("stats::basic variance","[weight=1][part=stats]"){
    PNG data; data.resize(2,2);
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 2; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = 20 * i + 3 * j;
            p->g = 3 * i + 20 * j;
            p->b = 23 * i + 23 * j;
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(0,0);
    long result = s.getVar(ul,1);

    REQUIRE(result == 1876);
}


TEST_CASE("qtcount::basic ctor render","[weight=1][part=qtcount]"){
    PNG img;
    img.readFromFile("images/orig/geo.png");

    qtcount t1(img);

    PNG out = t1.render();
    out.writeToFile("images/result/geo_ctor_render.png");
    REQUIRE(out==img);
}


TEST_CASE("qtcount::basic copy","[weight=1][part=qtcount]"){
    PNG img;
    img.readFromFile("images/orig/geo.png");

    qtcount t1(img);
    qtcount t1copy(t1);

    PNG out = t1copy.render();
    out.writeToFile("images/result/geo_copy.png");
    REQUIRE(out==img); 
}

TEST_CASE("qtcount::basic prune","[weight=1][part=qtcount]"){
    PNG img;
    img.readFromFile("images/orig/adasquare.png");
    
    qtcount t1(img); 
    t1.prune(3000);
    PNG result = t1.render();

    //result.writeToFile("images/soln/given-adaPrune-count.png");

    PNG expected; 
    expected.readFromFile("images/soln/given-adaPrune-count.png");

    REQUIRE(expected==result);
}

TEST_CASE("qtvar::basic prune","[weight=1][part=qtvar]"){
    PNG img;
    img.readFromFile("images/orig/adasquare.png");
    
    qtvar t1(img); 
    t1.prune(3000);
    PNG result = t1.render();

    result.writeToFile("images/result/result-given-adaPrune-var.png");

    PNG expected; 
    expected.readFromFile("images/soln/given-adaPrune-var.png");

    REQUIRE(expected==result);
}

TEST_CASE("qtcount::basic pruneSize","[weight=1][part=qtcount]"){
    PNG img;
    img.readFromFile("images/orig/adasquare.png");
    
    qtcount t1(img); 
    int result = t1.pruneSize(3000);

    int expected = 9394;

    REQUIRE(expected==result);
}

TEST_CASE("qtvar::basic pruneSize","[weight=1][part=qtvar]"){
    PNG img;
    img.readFromFile("images/orig/adasquare.png");
    
    qtvar t1(img); 
    int result = t1.pruneSize(3000);

    int expected = 15547;

    REQUIRE(expected==result);
}

TEST_CASE("qtvar::basic pruneSize 32x32 with tol == 0","[weight=1][part=qtvar]"){
    PNG img;
    img.readFromFile("images/orig/geo.png");
    
    qtvar t1(img); 
    int result = t1.pruneSize(0);

    int expected = 0;

    REQUIRE(expected==result);
}


TEST_CASE("qtvar::basic idealPrune","[weight=1][part=qtvar]"){
    PNG img;
    img.readFromFile("images/orig/adasquare.png");
    
    qtvar t1(img); 
    int result = t1.idealPrune(13904);

    int expected = 3865;

    REQUIRE(expected==result);
}

TEST_CASE("qtcount::basic idealPrune","[weight=1][part=qtcount]"){
    PNG img;
    img.readFromFile("images/orig/adasquare.png");
    
    qtcount t1(img); 
    int result = t1.idealPrune(13904);

    int expected = 1366;

    REQUIRE(expected==result);
}