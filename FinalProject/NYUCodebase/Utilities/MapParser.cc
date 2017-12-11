#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

void readEntityData(ifstream& stream) {
    string line;
    string type;

    while(getline(stream, line)) {
        if(line == "") { break; } 

        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);

        if(key == "type") {
            type="value";
        } else if(key =="location") {
            istringstream lineStream(value);
            string xPosition, yPosition;
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');

            float placeX = atoi(xPosition.c_str()) * TILE_SIZE;
            float placeY = atoi(yPosition.c_str()) * -TILE_SIZE;
        }
    }
}

void readLayerData(vector<vector<int>>& world, ifstream& stream) {
    string line;
    while(getline(stream, line)) {
        if(line =="") {break;} 
        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "data") {
            for(int y = 0; y < world.size()-1; ++y) {
                getline(stream, line);
                istringstream lineStream(line);
                string tile;

                for(int x = 0; x < world[y].size(); ++x) {
                    getline(lineStream, tile, ',');
                    int val = atoi(tile.c_str());
                    if(val > 0) {
                        world[y][x] = val-1;
                    } else {
                        world[y][x] = 0; 
                    }
                }
            } 
        }
    }
}

vector<vector<int>> readHeader(ifstream& stream) {
    string line;
    mapWidth = -1;
    mapHeight = -1;
    while(getline(stream, line)) {
        if(line=="") break; 

        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);

        if(key=="width") {
            mapWidth=atoi(value.c_str()); 
        } else if (key=="height") {
            mapHeight =atoi(value.c_str()); 
        }
    }
    vector<vector<int>> world(mapHeight, vector<int>(mapWidth, 0));
    return world;
}

void parseFile(string filename) {
    ifstream infile(filename);
    if(!infile) {
        cerr << "Failed to open: " << filename << endl; 
        exit(-1);
    }
    string line;
    while(getline(infline, line)) {
        if(line == "[header]") {
            if(!readHeader(infile)) {
                assert(false); 
            } 
        } else if (line=="[layer]") {
            readLayerData(infile);
        } else if(line == "[ObjectsLayer]") {
            readEntityData(infile); 
        }
    }
}
