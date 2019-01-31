weather="sunny"
func()
{
  return $(($1+$2))
}
func 3 5
echo $?
