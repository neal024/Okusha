#!/bin/bash
# OSHUKA:
# Deployment Script for qemu emulation
###

RETVAL=0
DEFLINK=IKNOW

usage()
{
    echo "Usage: ./deploy.sh {test|run}"
    echo "Usage: ./deploy.sh {test|run} {iknow|osdev}"
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

case "$2" in
    iknow) DEFLINK=IKNOW; ;;
    osdev) DEFLINK=OSDEV; ;;
    "")    DEFLINK=IKNOW; ;;
    *) usage ; RETVAL=2 ; exit $RETVAL;;
esac

case "$1" in
    test) deploy_kernel; RETVAL=$? ;;
    run) deploy_image; RETVAL=$? ;;
    *) usage ; RETVAL=2 ;;
esac

exit $RETVAL
