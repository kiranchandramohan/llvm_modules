#define IMG_SIZE 4096
#define HISTO_SIZE 256
#define NUM_ITER 30

void histo(int* image, int* histogram, int* gray_level_mapping, int start_indx, int end_indx)
{
  int i, j ;
  float cdf, pixels ;

  /* Initialize the histogram array. */

  for (i = start_indx ; i < end_indx ; i++) {
    for (j = 0; j < IMG_SIZE; ++j) {
      image[i*IMG_SIZE+j] = (i*j) % 255 ;
    }
  }

  for (i = 0 ; i < HISTO_SIZE ; i++)
    histogram[i] = 0;

  /* Compute the image's histogram */
  for (i = start_indx; i < end_indx ; i++) {
    for (j = 0; j < IMG_SIZE; ++j) {
      int pix = image[i*IMG_SIZE+j] ;
      histogram[pix] += 1;
    }
  }

  /* Compute the mapping from the old to the new gray levels */

  cdf = 0.0;
  pixels = (float) (IMG_SIZE * IMG_SIZE);
  for (i = 0; i < HISTO_SIZE; i++) {
    cdf += ((float)(histogram[i])) / pixels;
    gray_level_mapping[i] = (int)(255.0 * cdf);
  }

  /* Map the old gray levels in the original image to the new gray levels. */

  for (i = start_indx ; i < end_indx ; i++) {
    for (j = 0; j < IMG_SIZE; ++j) {
      image[i*IMG_SIZE+j] = gray_level_mapping[image[i*IMG_SIZE+j]];
    }
  }
}

int image[IMG_SIZE*IMG_SIZE] ;
int histogram[HISTO_SIZE] ;
int gray_level_mapping[HISTO_SIZE] ;

int main()
{
  int nIter = 0 ;
  for(nIter=0 ; nIter<NUM_ITER ; nIter++) {
    histo(image,histogram,gray_level_mapping,0,IMG_SIZE) ;
  }
  return 0 ;
}

