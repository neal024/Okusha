################################################################################
# !/bin/bash
# OSHUKA: Deployment Script for qemu emulation
################################################################################

RETVAL=0

usage()
{
    echo "Usage: ./deploy.sh {test|run|install}"
    RETVAL=1
}

deploy_kernel()
{
    #Run with Qemu
    make clean
    make DEFLINK=$DEFLINK
    qemu-system-i386 -kernel okusha/boot/kernel.bin
}

deploy_image()
{
    #Deploy as image
    make clean
    make build DEFLINK=$DEFLINK
    qemu-system-i386 -cdrom okusha.iso
}

deploy_install()
{
    #Deploy as image
    make clean
    make build DEFLINK=$DEFLINK
	make install
}

case "$1" in
    test) deploy_kernel; RETVAL=$? ;;
    run) deploy_image; RETVAL=$? ;;
	install) deploy_install; RETVAL=$? ;;
    *) usage ; RETVAL=2 ;;
esac

exit $RETVAL
