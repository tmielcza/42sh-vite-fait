echo "Single Quote"
echo 'foo
bar'
echo 'foo
bar'
echo 'foo\
bar'

echo "Double Quote"
echo "foo
bar"
echo "foo
bar"
echo "foo\
bar" 

echo "Double Quote Single Quote"
echo "echo 'foo
bar'"
echo "echo 'foo
bar'"
echo "echo 'foo\
bar'"

echo "Unusuel bytes"
echo 'uvwx'
echo 'uv		 wx'
echo " \8\9	898989"

echo "Escaping"
echo '\'abcd\''
echo "\'abcd\'"
echo "\\'abcd\\'"
echo "'"
echo \'
echo "'\''"
echo '\''
echo "'\'abcd\''"

# why does this work, this list was born of frustration, I tried
# everything I could think of.
echo \'{test//"'"/\'\\\'\'}\'" "
echo "'{test//"'"/}'"
echo "'{test//"'"/"'\\''"}'"
echo "'{test//"'"/\'\\'\'}'"
echo "'{test//'/}'"
echo "'{test//"'"/'\\''}'" # hangs waiting for '
echo "{test//str/"dddd"}"
echo {test//str/"dddd"}
echo {test//str/"'"}
echo "{test//str/"'"}"
echo "{test//str/'}"   # hangs
echo "{test//"'"/"'"}"
