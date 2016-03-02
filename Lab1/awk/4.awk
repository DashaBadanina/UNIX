BEGIN{
file=""
i=0
}
{
 if (file!=FILENAME)
{
  file=FILENAME
  split(FILENAME,array,"_")
  i++
  res[i]=array[2]
}
asort(res, sorted)
}
END{
for (i in sorted) print sorted[i]
}
