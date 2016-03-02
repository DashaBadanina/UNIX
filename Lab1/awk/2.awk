BEGIN{
}
{
  sum[FILENAME]+=$2
  indx[FILENAME]+=1
}
END{
 for(i in sum) {print( i ":" sum[i]/indx[i])}
}
