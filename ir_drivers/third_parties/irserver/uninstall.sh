#! /bin/sh

INSTALLDIR=/usr/local/bin
export INSTALLDIR

rm -fr $INSTALLDIR/remotes

rm -f $INSTALLDIR/irserver
rm -f $INSTALLDIR/irserver64
rm -f $INSTALLDIR/irclient
rm -f $INSTALLDIR/irclient64
rm -f $INSTALLDIR/ip_assign
rm -f $INSTALLDIR/ip_assign64

echo "IRTrans Software uninstalled"
ls -la  $INSTALLDIR
