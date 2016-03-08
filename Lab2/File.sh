#!/bin/bash

function findfile {

   x=1
   while [ $x -gt 0 ]
   do
    echo -n "Введите имя файла: "
    read FN
    if [ -z $FN ] 
     then
       x=0
     else
       if  test -e $FN
        then
          x=0
        else
          echo "Такого файла нет, попробуйте ввести другое имя"
          x=1
        fi
    fi
   done

}
function delete {

 findfile
 if !([ -z $FN ]) 
   then 
    rm $FN
    echo "Файл "$FN" удален"
 fi
}

function rename {

 findfile
 if !([ -z $FN ]) 
   then 
    echo -n "Введите новое имя файла файла: "
    read FNNew
    mv $FN $FNNew
    echo "Файл "$FN" переименован в "$FNNew
 fi
    
}
OPTIONS="ListFile DeleteFile RenameFile Exit"
select opt in ${OPTIONS}
do
case $opt in
 "ListFile")
    echo `ls`
   ./File.sh
   exit
 ;;
 "DeleteFile")
  delete
  ./File.sh
  exit
 ;;
 "RenameFile")
  rename
  ./File.sh
  exit
 ;;
 "Exit")
   exit
 ;;
esac
done
