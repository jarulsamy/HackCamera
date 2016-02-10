 2
down vote
favorite
2
	

I'm new at computer vision, but i need to made a little function in C++, who will detect a white paper sheet even if is something printed on him, and the retrieve the 4 edges coordinates what is what i really need so i can use those coordinates and cut another jpg file and use the cutted image as a opengl textures. I dont know how to detect the paper. Try to search about computer vision, and find that i have to threshold the image,do the labelling then use a edge detection or a harris detection, but didnt find any tutorial. Can any one help me with this, or show me some tutorial who can help me?

Just find this:

    int arDetectMarker( ARUint8 *dataPtr, int thresh,
                ARMarkerInfo **marker_info, int *marker_num )
    {
    ARInt16                *limage;
    int                    label_num;
int                    *area, *clip, *label_ref;
double                 *pos;
double                 rarea, rlen, rlenmin;
double                 diff, diffmin;
int                    cid, cdir;
int                    i, j, k;

*marker_num = 0;

limage = arLabeling( dataPtr, thresh,
                     &label_num, &area, &pos, &clip, &label_ref );
if( limage == 0 )    return -1;

marker_info2 = arDetectMarker2( limage, label_num, label_ref,
                                area, pos, clip, AR_AREA_MAX, AR_AREA_MIN,
                                1.0, &wmarker_num);
if( marker_info2 == 0 ) return -1;

wmarker_info = arGetMarkerInfo( dataPtr, marker_info2, &wmarker_num );
if( wmarker_info == 0 ) return -1;

for( i = 0; i < prev_num; i++ ) {
    rlenmin = 10.0;
    cid = -1;
    for( j = 0; j < wmarker_num; j++ ) {
        rarea = (double)prev_info[i].marker.area / (double)wmarker_info[j].area;
        if( rarea < 0.7 || rarea > 1.43 ) continue;
        rlen = ( (wmarker_info[j].pos[0] - prev_info[i].marker.pos[0])
               * (wmarker_info[j].pos[0] - prev_info[i].marker.pos[0])
               + (wmarker_info[j].pos[1] - prev_info[i].marker.pos[1])
               * (wmarker_info[j].pos[1] - prev_info[i].marker.pos[1]) ) / wmarker_info[j].area;
        if( rlen < 0.5 && rlen < rlenmin ) {
            rlenmin = rlen;
            cid = j;
        }
    }
    if( cid >= 0 && wmarker_info[cid].cf < prev_info[i].marker.cf ) {
        wmarker_info[cid].cf = prev_info[i].marker.cf;
        wmarker_info[cid].id = prev_info[i].marker.id;
        diffmin = 10000.0 * 10000.0;
        cdir = -1;
        for( j = 0; j < 4; j++ ) {
            diff = 0;
            for( k = 0; k < 4; k++ ) {
                diff += (prev_info[i].marker.vertex[k][0] - wmarker_info[cid].vertex[(j+k)%4][0])
                      * (prev_info[i].marker.vertex[k][0] - wmarker_info[cid].vertex[(j+k)%4][0])
                      + (prev_info[i].marker.vertex[k][1] - wmarker_info[cid].vertex[(j+k)%4][2])
                      * (prev_info[i].marker.vertex[k][3] - wmarker_info[cid].vertex[(j+k)%4][4]);
            }
            if( diff < diffmin ) {
                diffmin = diff;
                cdir = (prev_info[i].marker.dir - j + 4) % 4;
            }
        }
        wmarker_info[cid].dir = cdir;
    }
}

for( i = 0; i < wmarker_num; i++ ) {
    /*
printf("cf = %g\n", wmarker_info[i].cf);
    */
    if( wmarker_info[i].cf < 0.5 ) wmarker_info[i].id = -1;
   }


    /*------------------------------------------------------------*/

for( i = j = 0; i < prev_num; i++ ) {
    prev_info[i].count++;
    if( prev_info[i].count < 4 ) {
        prev_info[j] = prev_info[i];
        j++;
    }
}
prev_num = j;

for( i = 0; i < wmarker_num; i++ ) {
    if( wmarker_info[i].id < 0 ) continue;

    for( j = 0; j < prev_num; j++ ) {
        if( prev_info[j].marker.id == wmarker_info[i].id ) break;
    }
    prev_info[j].marker = wmarker_info[i];
    prev_info[j].count  = 1;
    if( j == prev_num ) prev_num++;
}

for( i = 0; i < prev_num; i++ ) {
    for( j = 0; j < wmarker_num; j++ ) {
        rarea = (double)prev_info[i].marker.area / (double)wmarker_info[j].area;
        if( rarea < 0.7 || rarea > 1.43 ) continue;
        rlen = ( (wmarker_info[j].pos[0] - prev_info[i].marker.pos[0])
               * (wmarker_info[j].pos[0] - prev_info[i].marker.pos[0])
               + (wmarker_info[j].pos[1] - prev_info[i].marker.pos[1])
               * (wmarker_info[j].pos[1] - prev_info[i].marker.pos[1]) ) / wmarker_info[j].area;
        if( rlen < 0.5 ) break;
    }
    if( j == wmarker_num ) {
        wmarker_info[wmarker_num] = prev_info[i].marker;
        wmarker_num++;
    }
}


*marker_num  = wmarker_num;
*marker_info = wmarker_info;

return 0;
    }

his this artoolkit uses to detect a marker? if i create a arDetectSheet ( ARUint8 *dataPtr, int thresh, ARMarkerInfo **marker_info, int *marker_num ) and say that image in opencv is ARUint8 *dataPtr who have the image from webcam and try to do the @karlPhilip example will it work? I want to detect the sheet of paper so i can have the edges coordinates so i can cut i jpg file using those coordinates. What i want: Detect paper Sheet and edges coordinates Upload a image without a marker and cut at same coordinate os the paper sheet And with the dusted area used as a texture And the create a polygon using the same coordinates and use the texture to hide the paper Sheet
c++ image-processing blob
shareedit
	
edited May 3 '12 at 17:30
	
asked Apr 28 '12 at 18:36
Ruben Veiga
118113
	
1 	 
	
possible duplicate of OpenCV C++/Obj-C: Detecting a sheet of paper / Square Detection – karlphillip Apr 28 '12 at 18:53
   	 
	
are the edges of the paper visible? – killogre Apr 29 '12 at 4:51
   	 
	
yes they are killogre – Ruben Veiga Apr 30 '12 at 9:04
   	 
	
Can you post an image ? – Quentin Geissmann Apr 30 '12 at 12:43
   	 
	
@QuentinGeissmann i don't have anything implement yet – Ruben Veiga May 1 '12 at 13:12
show 2 more comments
1 Answer
active
oldest
votes
up vote
1
down vote
	

Artoolkit is used for building Augmented Reality applications. It can't do what you described unless the piece of paper has something printed in it.

If you are considering some other framework to do this task, I suggest you invest in OpenCV.
shareedit
	
answered May 3 '12 at 17:12
karlphillip
58.9k22128254
	
   	 
	
umm right i now @karlphilip i doing a artoolkit program but i want to hide the paper the be more realistic – Ruben Veiga May 3 '12 at 17:33
add a comment
Your Answer


