#include <iostream>
#include <fstream>
#include <sstream>
#include<bits/stdc++.h>
#include <math.h>
#include <limits.h>
#include <vector>

#include </home/horej024/csci5607/assignment1/glm/glm.hpp>  //set to correct directory

class FovType {
    public:
    float h;
    float v;
};

class ImsizeType{
    public:
    int width;
    int height;
};

class ViewingwindowType{
    public:
    int height;
    int width;
    glm::vec3 ul;
    glm::vec3 ur;
    glm::vec3 ll;
    glm::vec3 lr;
};

class ColorType{
    public:
    float r;
    float g;
    float b;
};

class RayType{
    public:
    glm::vec3 position;
    glm::vec3 direction;
};

class SphereType {
    public:
    float radius;
    glm::vec3 position;
    ColorType m;
};

ColorType Shade_ray(SphereType sphere){
    return sphere.m;
}

ColorType Trace_Ray(RayType ray, std::vector<SphereType> objects, ColorType backgroud){
    float min_d = FLT_MAX;
    int current_obj = -1; 
    int iter = -1;
    for(auto sphere : objects){
        iter += 1; 
        float xc = ray.position.x - sphere.position.x;
        float yc = ray.position.y - sphere.position.y;
        float zc = ray.position.z - sphere.position.z;
        float A = 1.0;
        float B = 2 * (ray.direction.x * (xc) + ray.direction.y * (yc) + ray.direction.z * (zc));
        float C = (xc) * (xc) + (yc) * (yc) + (zc) * (zc) - sphere.radius * sphere.radius;
        float disc = (B * B) - (4 * C);
        if(disc > 0){
            float t1 = (-B + sqrt(disc)) / (2 * A);
            float t2 = (-B - sqrt(disc)) / (2 * A);
            if(t1 > 0 && t1 < min_d){
                min_d = t1;
                current_obj = iter;
            }
            if(t2 > 0 && t2 < min_d){
                min_d = t1;
                current_obj = iter;
            }
        }
        else if(disc == 0){
            float t1 = (-B / 2 * A);
            if(t1 > 0 && t1 < min_d){
                min_d = t1;
                current_obj = iter;
            }

        }
    }
    if(current_obj != -1){
        return Shade_ray(objects.at(current_obj));
    }
    return backgroud;
}

bool is_number(const std::string& s){ //determines if a string is a number.
    for(char const &ch : s){
        if(std::isdigit(ch) == 0 && ch != '-' && ch != '.'){
            return false;
        }
    }
    return true;
}

int raycast(std::string filename){
    std::ifstream file;
    file.open(filename);
    if(file.fail()){
        std::cout << "file fail\n";
        return 0;
    }
    if(filename.substr(filename.size() - 4) != ".txt"){
        std::cout << "not a text file!\n";
        return 0;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::vector<SphereType> objects;
    std::vector<ColorType> object_colors;
    glm::vec3 eye;
    glm::vec3 vdir;
    glm::vec3 up;
    FovType fov;
    ImsizeType imsize;
    ColorType bkgcolor;
    ViewingwindowType window;
    RayType ray;
    while(buffer){  // 1. read information from text file
        std::string subs;
        buffer >> subs;
        if(subs == "eye"){  // get eye information.
            buffer >> subs;
            if(is_number(subs)){
                eye.x = stof(subs);
            }
            else{
                return 0;
            }
            buffer >> subs;
            if(is_number(subs)){
                eye.y = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                eye.z = stof(subs);
            }
            else return 0;
        }
        else if(subs == "viewdir"){  // get viewdir information.
            buffer >> subs;
            if(is_number(subs)){
                vdir.x = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                vdir.y = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                vdir.z = stof(subs);
            } 
            else return 0;
        }
        else if(subs == "updir"){    // get updir information.
            buffer >> subs;
            if(is_number(subs)){
                up.x = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                up.y = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                up.z = stof(subs);
            }
            else return 0;
        }
        else if(subs == "hfov"){ // get fov information.
            buffer >> subs;
            if(is_number(subs)){
                fov.h = stof(subs);
            }
            else return 0;
        }
        else if(subs == "imsize"){   // get size information
            buffer >> subs;
            if(is_number(subs)){
                imsize.width = stoi(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                imsize.height = stoi(subs);
            }
            else return 0;
        }
        else if(subs == "bkgcolor"){   // get background color information
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                bkgcolor.r = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                bkgcolor.g = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                bkgcolor.b = stof(subs);
            }
            else return 0;
        }
        else if(subs == "mtlcolor"){   // get background color information
            buffer >> subs;
            ColorType newcolor;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newcolor.r = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newcolor.g = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newcolor.b = stof(subs);
            }
            else return 0;
            object_colors.push_back(newcolor);
        }
        else if(subs == "sphere"){
            buffer >> subs;
            SphereType newsphere;
            if(is_number(subs)){
                newsphere.position.x = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                newsphere.position.y = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                newsphere.position.z = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                newsphere.radius = stof(subs);
            }
            else return 0;
            objects.push_back(newsphere);
        }
    }
    if(objects.size() != object_colors.size()){
        return 0;
    }
    else{
        for(int i = 0; i < objects.size(); i++){
            objects.at(i).m = object_colors.at(i);
        }
    }
    ColorType arr[imsize.width * imsize.height];  // 2. Define an array of sufficent size to store the color values
    float aspect_ratio = (float)imsize.width/(float)imsize.height;  // define viewing window
    window.width = 2 * 5 * round(tan(.5 * (fov.h * M_PI / 180.0)));
    window.height = window.width / aspect_ratio;

    glm::vec3 u = glm::cross(vdir, up); // calculate u and v
    glm::vec3 v = glm::cross(u, vdir);
    if(u.x == -0){     //cross managed to compute -0, this is to fix incase.
        u.x = 0;
    }
    if(u.y == -0){
        u.y = 0;
    }
    if(u.z == -0){
        u.z = 0;
    }
    if(v.x == -0){
        v.x = 0;
    }
    if(v.y == -0){
        u.y = 0;
    }
    if(v.z == -0){
        v.z = 0;
    }
    glm::vec3 u2;
    u2 = u * (float)(window.width/2);
    glm::vec3 v2;
    v2 = v * (float)(window.height/2);
    window.ul = eye + vdir * (float)5 - u2 + v2;    // calculate four courners of viewing window
    window.ur = eye + vdir * (float)5 + u2 + v2;
    window.ll = eye + vdir * (float)5 - u2 - v2;
    window.lr = eye + vdir * (float)5 + u2 - v2;
    glm::vec3 deltaH;
    glm::vec3 deltaV;
    deltaH = (window.ur - window.ul) / (float)(imsize.width -1);
    deltaV = (window.ll - window.ul) / (float)(imsize.height -1);
    glm::vec3 point;
    std::string newfile = filename; // A string storing the output file's name is created and assigned with the previous file name, .txt is removed and then out.txt is appended.
    newfile.pop_back();
    newfile.pop_back();
    newfile.pop_back();
    newfile.pop_back();
    newfile.append(".ppm");
    std::ofstream outfile(newfile); // create new file with our created name.
    outfile << "P3" << "\n" << imsize.width << " " << imsize.height << "\n" << "255\n"; //write header
    ray.position = eye;
    for(int j = 0; j < imsize.height; j++){  // 4. for each pixel in the output image
        for(int i = 0; i < imsize.width; i++){
            point = window.ul + (float)i * deltaH + (float)j * deltaV;
            ray.direction = (point - eye) / sqrt(((point.x - eye.x) * (point.x - eye.x)) + ((point.y - eye.y) * (point.y - eye.y)) + ((point.z - eye.z) * (point.z - eye.z)));
            arr[i] = Trace_Ray(ray, objects, bkgcolor);
            outfile << arr[i].r * 255 << " " << arr[i].g * 255 << " " << arr[i].b * 255 << "\n";
        }
    }
    outfile.close();
    return 1;
}

int main(int argc, char **argv){
    if(argc == 2){
    raycast(argv[1]);
    }
    else{
        std::cout << "One file needed\n";
        return 0;
    }
}
