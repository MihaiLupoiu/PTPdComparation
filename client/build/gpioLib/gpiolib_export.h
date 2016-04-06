
#ifndef GPIOLIB_EXPORT_H
#define GPIOLIB_EXPORT_H

#ifdef GPIOLIB_STATIC_DEFINE
#  define GPIOLIB_EXPORT
#  define GPIOLIB_NO_EXPORT
#else
#  ifndef GPIOLIB_EXPORT
#    ifdef gpioLib_EXPORTS
        /* We are building this library */
#      define GPIOLIB_EXPORT 
#    else
        /* We are using this library */
#      define GPIOLIB_EXPORT 
#    endif
#  endif

#  ifndef GPIOLIB_NO_EXPORT
#    define GPIOLIB_NO_EXPORT 
#  endif
#endif

#ifndef GPIOLIB_DEPRECATED
#  define GPIOLIB_DEPRECATED __attribute__ ((__deprecated__))
#  define GPIOLIB_DEPRECATED_EXPORT GPIOLIB_EXPORT __attribute__ ((__deprecated__))
#  define GPIOLIB_DEPRECATED_NO_EXPORT GPIOLIB_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define GPIOLIB_NO_DEPRECATED
#endif

#endif
