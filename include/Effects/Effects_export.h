
#ifndef EFFECTS_EXPORT_H
#define EFFECTS_EXPORT_H

#ifdef EFFECTS_STATIC_DEFINE
#  define EFFECTS_EXPORT
#  define EFFECTS_NO_EXPORT
#else
#  ifndef EFFECTS_EXPORT
#    ifdef Effects_EXPORTS
        /* We are building this library */
#      define EFFECTS_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define EFFECTS_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef EFFECTS_NO_EXPORT
#    define EFFECTS_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef EFFECTS_DEPRECATED
#  define EFFECTS_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef EFFECTS_DEPRECATED_EXPORT
#  define EFFECTS_DEPRECATED_EXPORT EFFECTS_EXPORT EFFECTS_DEPRECATED
#endif

#ifndef EFFECTS_DEPRECATED_NO_EXPORT
#  define EFFECTS_DEPRECATED_NO_EXPORT EFFECTS_NO_EXPORT EFFECTS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef EFFECTS_NO_DEPRECATED
#    define EFFECTS_NO_DEPRECATED
#  endif
#endif

#endif /* EFFECTS_EXPORT_H */
