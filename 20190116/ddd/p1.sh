p="helloworld"
echo 'hello' "${p}p"
unset p;
echo "hello$p"
echo $(pwd)
echo $0 $1 $2
for x in one two three
do
   echo "number $x"
done
