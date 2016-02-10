/****************************************************************************/
/*                        Edge Detection Program                            */ 
/*  A suggested user interface is as follows:                               */   
/*  edge {-roberts,-prewitt,-sobel,-frei} [-skipNMS] [-t thresh1 thresh2] img > edgem*/
/*                    ECE 532 : Digital Image Analysis                      */
/*                              HW Assignment 1                             */
/*  Input: PGM file
    Output:PGM file + Image map.
    Author: Nikhil Shirahatti
    Date: 09/13/2003

*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern int read_pgm_hdr(FILE *, int *, int *);
extern void **matrix(int, int, int, int, int);
extern void error(const char *);
int skipNMS=0;


/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/

/* READ PGM HEADER */

/* This function reads the header of a PGM image. */
/* The dimensions are returned as arguments. */
/* This function ensures that there's no more than 8 bpp. */
/* The return value is negative if there's an error. */

int read_pgm_hdr(FILE *fp, int *nrows, int *ncols)
{
  char filetype[3];
  int maxval;

  if(skipcomment(fp) == EOF
     || fscanf(fp, "%2s", filetype) != 1
     || strcmp(filetype, "P5")
     || skipcomment(fp) == EOF
     || fscanf(fp, "%d", ncols) != 1
     || skipcomment(fp) == EOF
     || fscanf(fp, "%d", nrows) != 1
     || skipcomment(fp) == EOF
     || fscanf(fp, "%d%*c", &maxval) != 1
     || maxval > 255)
    return(-1);
  else return(0);
}

/*-----------------------------------------------------------------------------------*/
/* ERROR HANDLER */

void error(const char *msg)
{
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

/*------------------------------------------------------------------------------------*/

/* DYNAMICALLY ALLOCATE A PSEUDO 2-D ARRAY */

/* This function allocates a pseudo 2-D array of size nrows x ncols. */
/* The coordinates of the first pixel will be first_row_coord and */
/* first_col_coord. The data structure consists of one contiguous */
/* chunk of memory, consisting of a list of row pointers, followed */
/* by the array element values. */
/* Assumption:  nrows*ncols*element_size, rounded up to a multiple  */
/* of sizeof(long double), must fit in a long type.  If not, then */
/* the "i += ..." step could overflow.                              */

void **matrix(int nrows, int ncols, int first_row_coord,
              int first_col_coord, int element_size)
{
  void **p;
  int alignment;
  long i;

  if(nrows < 1 || ncols < 1) return(NULL);
  i = nrows*sizeof(void *);
  /* align the addr of the data to be a multiple of sizeof(long double) */
  alignment = i % sizeof(long double);
  if(alignment != 0) alignment = sizeof(long double) - alignment;
  i += nrows*ncols*element_size+alignment;
  if((p = (void **)malloc((size_t)i)) != NULL)
  {
    /* compute the address of matrix[first_row_coord][0] */
    p[0] = (char *)(p+nrows)+alignment-first_col_coord*element_size;
    for(i = 1; i < nrows; i++)
      /* compute the address of matrix[first_row_coord+i][0] */
      p[i] = (char *)(p[i-1])+ncols*element_size;
    /* compute the address of matrix[0][0] */
    p -= first_row_coord;
  }
  return(p);
}

/*----------------------------------------------------------------------------------------------------------------------*/
/* SKIP COMMENT */

/* This function skips past a comment in a file. The comment */
/* begins with a '#' character and ends with a newline character. */
/* The function returns EOF if there's an error. */

int skipcomment(FILE *fp)
{
  int i;

  if((i = getc(fp)) == '#')
    while((i = getc(fp)) != '\n' && i != EOF);
  return(ungetc(i, fp));
}


/*---------------------------------------------------------------------------------------------------------------------*/

/* REFLECT AN IMAGE ACROSS ITS BORDERS */

/* The parameter "amount" tells the number of rows or columns to be */
/* reflected across each of the borders. */
/* It is assumed that the data type is unsigned char. */
/* It is assumed that the array was allocated to be of size at least */
/* (nrows+2*amount) by (ncols+2*amount), and that the image was loaded */
/* into the middle portion of the array, with coordinates, */
/*      0 <= row < nrows, 0 <= col < ncols */
/* thereby leaving empty elements along the borders outside the image */
/* The "reflect" function will then fill in those empty */
/* elements along the borders with the reflected image pixel values. */
/* For example, x[0][-1] will be assigned the value of x[0][0], */
/* and x[0][-2] will be assigned the value of x[0][1], if amount=2. */

void reflect(unsigned char **xc, int nrows, int ncols, int amount)
{
  int i, j;

  if(matrix == NULL || nrows < 1 || ncols < 1 || amount < 1
    || amount > (nrows+1)/2 || amount > (ncols+1)/2)
    error("reflect: bad args");

  for(i = -amount; i < 0; i++)
  {
    for(j = -amount; j < 0; j++)
      xc[i][j] = xc[-i-1][-j-1];
    for(j = 0; j < ncols; j++)
      xc[i][j] = xc[-i-1][j];
    for(j = ncols; j < ncols+amount; j++)
      xc[i][j] = xc[-i-1][ncols+ncols-j-1];
  }
  for(i = 0; i < nrows; i++)
  {
    for(j = -amount; j < 0; j++)
      xc[i][j] = xc[i][-j-1];
    for(j = ncols; j < ncols+amount; j++)
      xc[i][j] = xc[i][ncols+ncols-j-1];
  }
  for(i = nrows; i < nrows+amount; i++)
  {
    for(j = -amount; j < 0; j++)
      xc[i][j] = xc[nrows+nrows-i-1][-j-1];
    for(j = 0; j < ncols; j++)
      xc[i][j] = xc[nrows+nrows-i-1][j];
    for(j = ncols; j < ncols+amount; j++)
      xc[i][j] = xc[nrows+nrows-i-1][ncols+ncols-j-1];
  }
}
/*---------------------------------------------------------------------------------------------*/

/* REFLECTING FLOAT */

void reflectf(float **xc, int nrows, int ncols, int amount)
{
 int i, j;

  if(matrix == NULL || nrows < 1 || ncols < 1 || amount < 1
    || amount > (nrows+1)/2 || amount > (ncols+1)/2)
    error("reflect: bad args");

  
  for(i = -amount; i < 0; i++)
  {
    for(j = -amount; j < 0; j++)
      xc[i][j] = xc[-i-1][-j-1];
    for(j = 0; j < ncols; j++)
      xc[i][j] = xc[-i-1][j];
    for(j = ncols; j < ncols+amount; j++)
      xc[i][j] = xc[-i-1][ncols+ncols-j-1];
  }
  
  for(i = 0; i < nrows; i++)
  {
    for(j = -amount; j < 0; j++)
      xc[i][j] = xc[i][-j-1];
    for(j = ncols; j < ncols+amount; j++)
      xc[i][j] = xc[i][ncols+ncols-j-1];
  }
  
  for(i = nrows; i < nrows+amount; i++)
  {
    for(j = -amount; j < 0; j++)
      xc[i][j] = xc[nrows+nrows-i-1][-j-1];
    for(j = 0; j < ncols; j++)
      xc[i][j] = xc[nrows+nrows-i-1][j];
    for(j = ncols; j < ncols+amount; j++)
      xc[i][j] = xc[nrows+nrows-i-1][ncols+ncols-j-1];
  }
}
/*-------------------------------------------------------------------------------------------------------------------*/

/* A method for Linear Interpolation */

float LinearInterpolate(
   float y1,float y2,
   float mu)
{
   return(y1*(1-mu)+y2*mu);
}

/*-------------------------------------------------------------------------------------------*/

void nms(float **anglec, float **magc,int nrows , int ncols,int thresh,int thresh2)
{

/* PERFORMS NON-MAXIMUM SUPRESSION TO DETECT EDGES */

  int i,j,r,c,edgepoints1=0,edgepoints2=0,last=0,nochange;
  FILE *fpy;
  float angle,maxm;
  int ax_pos,ay_pos,r1,c1,r2,c2,edgecount,r3,c3,r4,c4,count_points=0;
  unsigned char** edgemap;
  float mu =0.5 , anglecal, ivalue1,ivalue2;
  printf("============================================================\n");
  printf(" Performing Non_maximum-Supression\n");
  

  edgemap = (unsigned char **) matrix(nrows+2,ncols+2,-1,-1,sizeof(char));
  /* SO FIRST REFLECT THE MAGC BY 1 PIXEL */
  reflectf(magc, nrows, ncols, 1);
  reflectf(anglec,nrows,ncols,1);
  printf("-------------------------------------------------\n");
  printf("Finished processing: Reflected magnitude image by 1 pixel\n");
  printf("Finished processing: Reflected angle image by 1 pixel\n");

 /* HYSTERESIS THRESHODING */ 

      for(i=0;i<nrows;i++)
        {
          for(j=0;j<ncols;j++)
            {
              edgemap[i][j] =0;
            }
        }

      if(thresh != thresh2)
        {
          while( nochange)
            {
              
              for(i=0;i<nrows;i++)
                {
                  for(j=0;j<ncols;j++)
                    {
                      if(magc[i][j] >= thresh2 *thresh2)
                        {
                          edgemap[i][j] = 255;
                          edgepoints2++;
                        }
                      else if(magc[i][j] <= thresh*thresh)
                        {
                          edgemap[i][j] = 0;
                        }
                      else
                        {
                          edgecount =0;
                          for(r=i-1;r<= i+1; r++)
                            {
                              for(c=j-1;c<=j+1;c++)
                                {
                                  if(edgemap[r][c] == 255 )
                                    edgecount++;
                                }
                            }
                          if(edgecount >= 1)
                            {
                              edgemap[i][j] = 255;
                              edgepoints2++;
                            }
                          else
                            edgemap[i][j] =0;
                        }
                    }
                }
            
              /* CONDITION FOR CONVERGENCE */
              if(abs(last-edgepoints2) < 20)
                nochange=0; /* means there is nochange stop */
              else
                nochange =1; /* means there is change -> iterate */
              last = edgepoints2;
              count_points = count_points + edgepoints2;
              edgepoints2 =0;
            }
        }





          printf(" Number of Edgepoints after hysterisis thresholding is %d\n",last );
/*  if(count_points < 13000)
    {
      printf("===========================================================\n");
      printf(" Reduce lower threshold or Upper threshold and try again \n");
      exit(0);
    }
*/  
printf(" Finsihed calculating the edges using thresholding and NMS\n");

 /* WRITE THE IMAGE AFTER HYSTERISIS THRESHOLDING*/

  if((fpy =fopen("edgemap_nms_hyst.pgm","w"))== 0)
         error(" Error writing file\n");
  fprintf(fpy, "P5\n%d %d\n255\n", ncols, nrows);
  for(i = 0; i < nrows; i++)
    if(fwrite(&edgemap[i][0], sizeof(char), ncols, fpy) != ncols)
      error("can't write the image");
          
          
  /* FOR EACH PIXEL IN MAGC IF IT HAS A LOCAL MAXIMA IN THE DIRECTION OF ANGLEC
     THEN IT IS AN EDGE */
  
 

  for( i =0; i< nrows; i++)
    {
      for(j=0;j< ncols;j++)
        {
          angle = anglec[i][j];
              
          
          /* TO FIND POINTS FOR INTERPOLATION */
          /* printf("The angle is %f \n", angle); */
          /* BRUTE FORCE METHOD OF COMPARING EIGHT CASES */
          
          if( edgemap[i][j] == 255 )
            {

              anglecal = angle;

              if(0 <= anglecal < M_PI/4)
                {
                  r1=0;
                  c1=1;
                  r2 =1;
                  c2 =1;
                  r3 =0;
                  c3 =-1;
                  r4 = -1;
                  c4 -1;
                  /* mu = tan(angle); */
                }
              if(M_PI/4 <= anglecal <M_PI/2)
                {
                  r1=1;
                  c1=1;
                  r2 =1;
                  c2 =0;
                  r3 = -1;
                  c3 =-1;
                  r4 = -1;
                  c4 = 0;
                 /*  mu =1-1/tan(angle); */
                }
              if(M_PI/2 <= anglecal < 3*M_PI/4)
                {
                  r1=1;
                  c1=0;
                  r2 =1;
                  c2 =-1;
                  r3= -1;
                  c3 =0;
                  r4 = -1;
                  c4 =1;
                 /*  mu = -1/tan(angle); */
                } 
              if(3*M_PI/4 <= anglecal < M_PI)
                {
                  r1=1;
                  c1=-1;
                  r2 =0;
                  c2 =-1;
                  r3 =-1;
                  c3 =1;
                  r4 = 0;
                  c4 =1;
                 /*  mu = -tan(angle); */
              
                } 

              if(M_PI <= anglecal < - M_PI/4)
                {
                  r1=-1;
                  c1=1;
                  r2 =0;
                  c2 =1;
                  r3 =0;
                  c3 =-1;
                  r4 =1;
                  c4 = -1;
                  /* mu =tan(angle); */
                } 
              if(-M_PI/4 <= anglecal < -M_PI/2)
                {
                  r1=-1;
                  c1=1;
                  r2 =-1;
                  c2 =0;
                  r3 = 1;
                  c3 =-1;
                  r4 =1;
                  c4 =0;
                 /*  mu =1-1/tan(angle); */
                } 
              if(-M_PI/2 <= anglecal < -3*M_PI/4)
                {
                  r1=-1;
                  c1=0;
                  r2 =-1;
                  c2 =-1;
                  r3 = 1;
                  c3 =0;
                  r4=1;
                  c4 =1;
                  /* mu = -1/tan(angle); */
                }  
              if(-3*M_PI/4 <= anglecal < -M_PI)
                {
                  r1=-1;
                  c1=-1;
                  r2 =0;
                  c2 =-1;
                  r3= 1;
                  c3 =1;
                  r4=0;
                  c4 =1;
                 /*  mu = -tan(angle); */
                }  

              ivalue1 = LinearInterpolate(magc[i+r1][j+c1],magc[i+r2][j+c2],mu);
              ivalue2 =  LinearInterpolate(magc[i+r3][j+c3],magc[i+r4][j+c4],mu);
          
              /* END OF COMPARING ANGLES */
         
            
              if( magc[i][j] > ivalue2 && magc[i][j] > ivalue2)
                {
                  edgemap[i][j] = 255;
                  edgepoints1++;
                 
                }
              else
                {
                  edgemap[i][j] =0;
                }
            }
        }    
  

    }

  /* PRINT IMAGE AFTER NMS */
      printf(" Number of Edgepoints after NMS on hysterisis thresholding is %d\n", edgepoints1 );
      /* WRITE THE IMAGE */
  if((fpy =fopen("edgemap_nms.pgm","w"))== 0)
    error(" Error writing file\n");
  fprintf(fpy, "P5\n%d %d\n255\n", ncols, nrows);
  for(i = 0; i < nrows; i++)
    if(fwrite(&edgemap[i][0], sizeof(char), ncols, fpy) != ncols)
      error("can't write the image");
  fclose(fpy);
  
 /*  if((edgepoints1 > 12000 && edgepoints1 < 125000)) */
/*     { */
/*       printf(" # Edgepoints within the range specified\n"); */
/*     } */
/*   else if(edgepoints1 < 12000 ) */
/*     { */
/*       thresh = thresh -1; */
/*       thresh2 = thresh2 +1; */
/*       nms(anglec,magc,nrows,ncols,thresh,thresh2); */
/*     } */
/*   else */
/*     { */
/*       thresh = thresh + 1; */
/*       thresh2 = thresh2 +1; */
/*       nms(anglec,magc,nrows,ncols,thresh,thresh2); */
/*     } */
/*   printf("The lower threshold used = %d \n",thresh); */
/*    printf("The Upper threshold used = %d \n",thresh2); */
/*    printf("==========================================\n"); */
/* } */
}


/*---------------------------------------------------------------------------*/

/* EDGE DETECTION BY ROBERTS OPERATOR */

/* The yc array is where the magnitude of the resultant correlation is stored*/
/* The zc array is where the gradient of the resultant correlation is stored*/
/* The edgec array is where edge/ not an edge info is stored in 1's or 0's */

void robert(unsigned char **xc, int nrows, int ncols, int thresh, int thresh2)
{  
  
  float **row, **col,**theta,**y;
  unsigned char **edgemap;
  int i,j,edgepoints=0;
  FILE *fpy;
  
  if((fpy =fopen("edgemap_roberts.pgm","w"))== 0)
    error(" Error writing file\n");
  printf("-------------------------------------------------------------------\n");
  printf(" Applying Robert Operator\n");
  
  /* COMPUTE THE ROW COMPONENT */
   row = (float **)matrix(nrows, ncols, 0, 0, sizeof(float));
   col = (float **)matrix(nrows, ncols, 0, 0, sizeof(float));
   theta = (float **)matrix(nrows+2, ncols+2, -1, -1, sizeof(float));
   y = (float **)matrix(nrows+2, ncols+2, -1, -1, sizeof(float));
   edgemap = (unsigned char **) matrix(nrows,ncols,0,0,sizeof(char));
   
   if( row == NULL || col == NULL || theta == NULL || y == NULL || edgemap == NULL)
     error(" Allocation error of matrices in Robet's sub-function\n");

   printf("------------------------------------------\n");
   printf(" Allocated temporary arrays\n");
   for(i = 0; i < nrows; i++)
     {
     for(j = 0; j < ncols; j++)
       {
         row[i][j]= (float)(xc[i][j] - xc[i-1][j-1])/sqrt(2);
         col[i][j]= (float)(xc[i-1][j]-xc[i][j-1])/sqrt(2);
         y[i][j] = row[i][j]*row[i][j]+ col[i][j]*col[i][j];
         theta[i][j]= atan2(col[i][j],row[i][j]);
       }
     }

   /* CALL NMS BEFORE DOING ABSOLUTE THRESHOLDING */
   if(!skipNMS)
     nms(theta,y,nrows,ncols,thresh,thresh2);

   /* DO THE THRESHOLDING TO COMPUTE THE EDGE PIXELS */
   for(i=0;i<nrows;i++)
     {
       for(j=0;j<ncols;j++)
         {
           if(y[i][j] >= thresh2*thresh2)
             {
               edgemap[i][j] = 255;
               edgepoints++;
             }
           else
             {
               edgemap[i][j] =0;
             }
         }
     }

  
   /* EDGE-POINTS */
   printf("----------------------------------------------------------------\n");
   printf(" Number of Edgepoints using simple thresholding is %d\n",edgepoints);
   /* WRITE THE IMAGE */
  fprintf(fpy, "P5\n%d %d\n255\n", ncols, nrows);
  for(i = 0; i < nrows; i++)
    if(fwrite(&edgemap[i][0], sizeof(char), ncols, fpy) != ncols)
      error("can't write the image");

  
   /* CLOSE FILE & QUIT */
  fclose(fpy);
  exit(0);

   
  
  
}

/*---------------------------------------------------------------------------*/

/* EDGE DETECTION BY PREWITS OPERATOR */

/* The yc array is where the magnitude of the resultant correlation is stored*/
/* The zc array is where the gradient of the resultant correlation is stored*/
/* The edgec array is where edge/ not an edge info is stored in 1's or 0's */

void prewit(unsigned char **xc, int nrows, int ncols, int thresh,int thresh2)
{  
  /* The yc array is where the magnitude of the resultant correlation is stored*/
/* The zc array is where the gradient of the resultant correlation is stored*/
/* The edgec array is where edge/ not an edge info is stored in 1's or 0's */

  
  float **row, **col,**theta,**y;
  unsigned char **edgemap;
  int i,j,edgepoints=0;
  FILE *fpy;
  
  if((fpy =fopen("edgemap_prewitt.pgm","w"))== 0)
    error(" Error writing file\n");
  printf("-------------------------------------------------------------------\n");
  printf(" Applying Prewitt's Operator\n");
  
  /* COMPUTE THE ROW COMPONENT */
   row = (float **)matrix(nrows, ncols, 0, 0, sizeof(float));
   col = (float **)matrix(nrows, ncols, 0, 0, sizeof(float));
   theta = (float **)matrix(nrows+2, ncols+2, -1, -1, sizeof(float));
   y = (float **)matrix(nrows+2, ncols+2, -1, -1, sizeof(float));
   edgemap = (unsigned char **) matrix(nrows,ncols,0,0,sizeof(char));
   
   if( row == NULL || col == NULL || theta == NULL || y == NULL || edgemap == NULL)
     error(" Allocation error of matrices in Prewit's sub-function\n");

   printf("------------------------------------------\n");
   printf(" Allocated temporary arrays\n");
   for(i = 0; i < nrows; i++)
     {
     for(j = 0; j < ncols; j++)
       {
         row[i][j]= (float)(xc[i+1][j-1] +xc[i+1][j]+ xc[i+1][j+1] - xc[i-1][j-1]-xc[i-1][j]-xc[i-1][j+1])/6;
         col[i][j]= (float)(xc[i+1][j+1]+xc[i][j+1]+xc[i-1][j+1]-xc[i-1][j-1]-xc[i][j-1]-xc[i+1][j-1])/6;
         y[i][j] = row[i][j]*row[i][j]+ col[i][j]*col[i][j];
         theta[i][j]= atan2(col[i][j],row[i][j]);
       }
     }

    /* CALL NMS BEFORE DOING ABSOLUTE THRESHOLDING */

   if(!skipNMS)
     nms(theta,y,nrows,ncols,thresh,thresh2);


   /* DO THE THRESHOLDING TO COMPUTE THE EDGE PIXELS */
   for(i=0;i<nrows;i++)
     {
       for(j=0;j<ncols;j++)
         {
           if(y[i][j] >= thresh2*thresh2)
             {
               edgemap[i][j] = 255;
               edgepoints++;
             }
           else
             {
               edgemap[i][j] =0;
             }
         }
     }


   /* EDGE-POINTS */
   printf("----------------------------------------------------------------\n");
   printf(" Number of Edgepoints using simple thresholding is %d\n",edgepoints);

   /* WRITE THE IMAGE */
 fprintf(fpy, "P5\n%d %d\n255\n", ncols, nrows);
  for(i = 0; i < nrows; i++)
    if(fwrite(&edgemap[i][0], sizeof(char), ncols, fpy) != ncols)
      error("can't write the image");

  /* CLOSE FILE & QUIT */
  fclose(fpy);  
  exit(0);

   
  
  
}

/*---------------------------------------------------------------------------*/

/* EDGE DETECTION BY SOBEL OPERATOR */

/* The yc array is where the magnitude of the resultant correlation is stored*/
/* The zc array is where the gradient of the resultant correlation is stored*/
/* The edgec array is where edge/ not an edge info is stored in 1's or 0's */

void sobel(unsigned char **xc, int nrows, int ncols, int thresh,int thresh2)
{  
  /* The yc array is where the magnitude of the resultant correlation is stored*/
/* The zc array is where the gradient of the resultant correlation is stored*/
/* The edgec array is where edge/ not an edge info is stored in 1's or 0's */

 
  
  float **row, **col,**theta,**y;
  unsigned char **edgemap;
  int i,j,edgepoints=0;
  FILE *fpy;
  
  if((fpy =fopen("edgemap_sobel.pgm","w"))== 0)
    error(" Error writing file\n");
  printf("-------------------------------------------------------------------\n");
  printf(" Applying Sobel Operator\n");
  
  /* COMPUTE THE ROW COMPONENT */
   row = (float **)matrix(nrows, ncols, 0, 0, sizeof(float));
   col = (float **)matrix(nrows, ncols, 0, 0, sizeof(float));
   theta = (float **)matrix(nrows+2, ncols+2, -1, -1, sizeof(float));
   y = (float **)matrix(nrows+2, ncols+2, -1, -1, sizeof(float));
   edgemap = (unsigned char **) matrix(nrows,ncols,0,0,sizeof(char));
   
   if( row == NULL || col == NULL || theta == NULL || y == NULL || edgemap == NULL)
     error(" Allocation error of matrices in Sobel's sub-function\n");

   printf("------------------------------------------\n");
   printf(" Allocated temporary arrays\n");
   for(i = 0; i < nrows; i++)
     {
     for(j = 0; j < ncols; j++)
       {
         row[i][j]= (float)(xc[i+1][j-1]+ 2*xc[i+1][j]+xc[i+1][j-1] - xc[i-1][j-1] - 2*xc[i-1][j] -xc[i-1][j+1])/8;
         col[i][j]= (float)(xc[i-1][j-1]+2 * xc[i][j-1]+xc[i+1][j-1] - xc[i-1][j+1] - 2*xc[i][j+1] - xc[i+1][j-1])/8;
         y[i][j] = row[i][j]*row[i][j]+ col[i][j]*col[i][j];
         theta[i][j]= atan2(col[i][j],row[i][j]);
       }
     }

   
    /* CALL NMS BEFORE DOING ABSOLUTE THRESHOLDING */
   if(!skipNMS)
     nms(theta,y,nrows,ncols,thresh,thresh2);


   /* DO THE THRESHOLDING TO COMPUTE THE EDGE PIXELS */
   for(i=0;i<nrows;i++)
     {
       for(j=0;j<ncols;j++)
         {
           if(y[i][j] >= thresh2*thresh2)
             {
               edgemap[i][j] = 255;
               edgepoints++;
             }
           else
             {
               edgemap[i][j] =0;
             }
         }
     }


   /* EDGE-POINTS */
   printf("----------------------------------------------------------------\n");
   printf(" Number of Edgepoints using simple thresholding is %d\n",edgepoints);

   /* WRITE THE IMAGE */
  fprintf(fpy, "P5\n%d %d\n255\n", ncols, nrows);
  for(i = 0; i < nrows; i++)
    if(fwrite(&edgemap[i][0], sizeof(char), ncols, fpy) != ncols)
      error("can't write the image");

  /* CLOSE FILE & QUIT */
  fclose(fpy); 
  exit(0);

   
  
}

/*---------------------------------------------------------------------------*/

/* EDGE DETECTION BY FRIE-CHEN OPERATOR */

/* The yc array is where the magnitude of the resultant correlation is stored*/
/* The zc array is where the gradient of the resultant correlation is stored*/
/* The edgec array is where edge/ not an edge info is stored in 1's or 0's */

void frie_chen(unsigned char **xc, int nrows, int ncols, int thresh,int thresh2)
{  
  /* The yc array is where the magnitude of the resultant correlation is stored*/
/* The zc array is where the gradient of the resultant correlation is stored*/
/* The edgec array is where edge/ not an edge info is stored in 1's or 0's */
 
  
  float **row, **col,**theta,**y;
  unsigned char **edgemap;
  int i,j,edgepoints =0;
  FILE *fpy;
  
  if((fpy =fopen("edgemap_frie_chen.pgm","w"))== 0)
    error(" Error writing file\n");
  printf("-------------------------------------------------------------------\n");
  printf(" Applying Frie-Chen Operator\n");
  
  /* COMPUTE THE ROW COMPONENT */
   row = (float **)matrix(nrows, ncols, 0, 0, sizeof(float));
   col = (float **)matrix(nrows, ncols, 0, 0, sizeof(float));
   theta = (float**)matrix(nrows+2, ncols+2, -1, -1, sizeof(float));
   y = (float **)matrix(nrows+2, ncols+2, -1, -1, sizeof(float));
   edgemap = (unsigned char **) matrix(nrows,ncols,0,0,sizeof(char));
   
   if( row == NULL || col == NULL || theta == NULL || y == NULL || edgemap == NULL)
     error(" Allocation error of matrices in Frie-Chen's sub-function\n");

   printf("------------------------------------------\n");
   printf(" Allocated temporary arrays\n");
   for(i = 0; i < nrows; i++)
     {
     for(j = 0; j < ncols; j++)
       {
         row[i][j]=(float)(xc[i+1][j-1]+ sqrt(2)*xc[i+1][j]+xc[i+1][j-1] - xc[i-1][j-1] - sqrt(2)*xc[i-1][j] -xc[i-1][j+1])/8 ;
         col[i][j]= (float)(xc[i-1][j-1]+  sqrt(2)* xc[i][j-1]+xc[i+1][j-1] - xc[i-1][j+1] - sqrt(2)*xc[i][j+1] - xc[i+1][j-1])/8;;
         y[i][j] = row[i][j]*row[i][j]+ col[i][j]*col[i][j];
         theta[i][j]= atan2(col[i][j],row[i][j]);
       }
     }


    /* CALL NMS BEFORE DOING ABSOLUTE THRESHOLDING */
    if(!skipNMS) 
      nms(theta,y,nrows,ncols,thresh,thresh2);

   
   /* DO THE THRESHOLDING TO COMPUTE THE EDGE PIXELS */
   for(i=0;i<nrows;i++)
     {
       for(j=0;j<ncols;j++)
         {
           if(y[i][j] >= thresh2*thresh2)
             {
               edgemap[i][j] = 255;
               edgepoints++;
             }
           else
             {
               edgemap[i][j] =0;
             }
         }
     }

/* EDGE-POINTS */
   printf("----------------------------------------------------------------\n");
   printf(" Number of Edgepoints using simple thresholding is %d\n",edgepoints);


   
   /* WRITE THE IMAGE */
 fprintf(fpy, "P5\n%d %d\n255\n", ncols, nrows);
  for(i = 0; i < nrows; i++)
    if(fwrite(&edgemap[i][0], sizeof(char), ncols, fpy) != ncols)
      error("can't write the image");

  /* CLOSE FILE & QUIT */
  fclose(fpy);
  exit(0);

   
  
}


/* ====================================================================================*/

int main(int argc, char **argv)
{
  FILE *fpx, *fpy;
  int nrows, ncols, i, j,thresh,thresh2;
  unsigned char **x;
  char *str;
  int r=0,p=0,s=0,f=0,histh=0;

  
  
  /* OPEN FILES */
  printf("------------------------------------------------\n");
  printf("Opening image file\n"); 
  if(argc == 0) fpx = stdin;
  else 
   {
     printf(" Making decesion\n");
     if(histh)
        {       
          if((fpx = fopen(*(++argv), "r")) == NULL)
          {             
                  printf("%s1\n",(*argv));
                  error("can't open file");
          }
        }               
        else
        {
          if((fpx = fopen((str), "r")) == NULL)
          {  
             printf("%s2\n",(*argv));
             error("can't open file");
          }
        }
   }    
  printf("-----------------------------------------\n");
  printf(" Opened file --image file %s \n", *argv);
  fpy = stdout;

  /* READ HEADER */
  if(read_pgm_hdr(fpx, &nrows, &ncols) < 0)
    error("not a PGM image or bpp > 8");
  printf("------------------------------------------\n");
  printf(" Read Header \n");

  fclose(fpx);
  return 1;
}

/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
