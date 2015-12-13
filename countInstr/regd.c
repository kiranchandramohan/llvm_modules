#define MAXGRID 64
#define LENGTH 2048
#define NUM_ITER 30

void reg_detect(int* sum_tang, int* mean, int* diff, int* sum_diff, int* path, int start_indx, int end_indx)
{
  int t, i, j, cnt;

  for (i = start_indx; i < end_indx; i++)
    for (j = 0; j < MAXGRID; j++) {
      sum_tang[i*MAXGRID+j] = (i+1)*(j+1);
      mean[i*MAXGRID+j] = (i-j)/MAXGRID;
      path[i*MAXGRID+j] = (i*(j-1))/MAXGRID;
    }

  for (j = start_indx; j <= end_indx - 1; j++)
    for (i = j; i <= MAXGRID - 1; i++)
      for (cnt = 0; cnt <= LENGTH - 1; cnt++) {
        diff[j*MAXGRID*LENGTH + i*LENGTH + cnt] =
          sum_tang[j*MAXGRID + i];
      }

  for (j = start_indx ; j <= end_indx - 1; j++)
  {
    for (i = j; i <= MAXGRID - 1; i++)
    {
      sum_diff[j*MAXGRID*LENGTH + i*LENGTH + 0] = diff[j*MAXGRID*LENGTH + i*LENGTH + 0] ;
      for (cnt = 1; cnt <= LENGTH - 1; cnt++) {
        sum_diff[j*MAXGRID*LENGTH + i*LENGTH + cnt] =
          sum_diff[j*MAXGRID*LENGTH + i*LENGTH + cnt-1] +
          diff[j*MAXGRID*LENGTH + i*LENGTH + cnt];
      }

      mean[j*MAXGRID+i] = sum_diff[j*MAXGRID*LENGTH + i*LENGTH + LENGTH-1];
    }
  }

  for (i = start_indx ; i <= end_indx - 1; i++)
    path[0*MAXGRID+i] = mean[0*MAXGRID+i];

  for (j = start_indx+1; j <= end_indx - 1; j++)
    for (i = j; i <= MAXGRID - 1; i++)
      path[j*MAXGRID+i] = path[(j-1)*MAXGRID+i-1] + mean[j*MAXGRID+i];
}

int sum_tang[MAXGRID*MAXGRID] ;
int mean[MAXGRID*MAXGRID] ;
int diff[MAXGRID*MAXGRID*LENGTH] ;
int sum_diff[MAXGRID*MAXGRID*LENGTH] ;
int path[MAXGRID*MAXGRID] ;

int main()
{
  int i ;
  for(i=0 ; i<NUM_ITER ; i++)
  {
    reg_detect(sum_tang, mean, diff, sum_diff, path, 0, MAXGRID) ;
  }
  return 0 ;
}

