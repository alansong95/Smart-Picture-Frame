#include <ctime>
#include <fstream>
#include <iostream>
#include <raspicam/raspicam.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

using namespace std;

int main ( int argc,char **argv ) {
    raspicam::RaspiCam Camera; //Cmaera object
    //Open camera 
    if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return -1;}
    //wait a while until camera stabilizes
    sleep(3);
    //capture

    
	string output;
	char * output_char;

	string take;
	char * take_char;
	string prev;
	char * prev_char;
	string next;
	char * next_char;

	string space = " ";
	string output_tok;
	
	string output_json;
	string take_postfix = ",\"prev\":0,\"next\":0}";

    while (1) {
	ifstream read("output");
        getline(read, output);
	output_char = (char *) output.c_str();

	read.close();

	// parsing JSON
	take = strtok(output_char, ",");
	prev = strtok(NULL, ",");
	next = strtok(NULL, "");

	take_char = (char *) take.c_str();
	prev_char = (char *) prev.c_str();
	next_char = (char *) next.c_str();

	take = strtok(take_char, ":");
	take = strtok(NULL, "");
	
	prev = strtok(prev_char, ":");
	prev = strtok(NULL, "");

	next = strtok(next_char, ":");
	next = strtok(NULL, "}");

	output_tok = take + space + prev + space + next + space;
	
        if (strcmp(take.c_str(), "1") == 0) {
	    output_char[8] = '0';
	    output_json = output_char + take_postfix;

	    ofstream write("output");
	    write << output_json.c_str();
	    write.close();	    

            Camera.grab();

	        //allocate memory
            unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];
            //extract the image in rgb format
            Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB );//get camera image
            //save
            std::ofstream outFile ( "sdl_display/image.ppm", std::ios::binary );
            outFile<<"P6\n"<< Camera.getWidth() <<" "<< Camera.getHeight() <<" 255\n";
            outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );

	    

            //free resrources    
            delete data;	   

	    ofstream write2("sdl_display/tok");
	    write2 << output_tok.c_str();
	    write2.close();
        } else if (strcmp(prev.c_str(), "1") == 0 || strcmp(next.c_str(), "1") == 0) {
	    ofstream write("output");
	    write << "{\"take\":0,\"prev\":0,\"next\":0}";
	    write.close();

	    ofstream write2("sdl_display/tok");
	    write2 << output_tok.c_str();
	    write2.close();

	}
	sleep(1);
    }
    return 0;
}
