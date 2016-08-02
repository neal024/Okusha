#!/bin/bash
# OSHUKA:
# Deployment Script for qemu emulation
###

RETVAL=0
usage()
{
    echo "Usage: ./deploy.sh {test|run}"
    RETVAL=1
}

deploy_kernel()
{
    #Run with Qemu
    make clean
    make
    qemu-system-i386 -kernel okusha/boot/kernel.bin
}

deploy_image()
{
    #Deploy as image
    make clean
    make build
    qemu-system-i386 -cdrom okusha.iso
}

case "$1" in
    test) deploy_kernel; RETVAL=$? ;;
    run) deploy_image; RETVAL=$? ;;
    *) usage ; RETVAL=2 ;;
esac

exit $RETVAL
