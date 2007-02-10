ACLOCAL=aclocal
AUTOCONF=autoconf
AUTOHEADER=autoheader
AUTOMAKE=automake
LIBTOOLIZE=libtoolize

echo "Running $ACLOCAL..."
$ACLOCAL
echo "Running $AUTOHEADER..."
$AUTOHEADER
echo "Running $LIBTOOLIZE --automake..."
$LIBTOOLIZE --automake
echo "Running $AUTOMAKE --gnu --add-missing --copy..."
$AUTOMAKE --add-missing
echo "Running $AUTOCONF..."
$AUTOCONF
