#! /bin/sh

INSTALLDIR=/usr/local/bin
export INSTALLDIR

mkdir $INSTALLDIR/remotes 2>/dev/null

cp irserver $INSTALLDIR
cp irserver64 $INSTALLDIR
chmod +x $INSTALLDIR/irserver
chmod +x $INSTALLDIR/irserver64
cp irclient $INSTALLDIR
cp irclient64 $INSTALLDIR
chmod +x $INSTALLDIR/irclient
chmod +x $INSTALLDIR/irclient64
cp ip_assign $INSTALLDIR
cp ip_assign64 $INSTALLDIR
chmod +x $INSTALLDIR/ip_assign
chmod +x $INSTALLDIR/ip_assign64

cp remotes/* $INSTALLDIR/remotes

echo "IRTrans Software installed in $INSTALLDIR"
ls -la  $INSTALLDIR
