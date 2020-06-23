// Generated by Apple Swift version 4.2.1 effective-4.1.50 (swiftlang-1000.11.42 clang-1000.11.45.1)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgcc-compat"

#if !defined(__has_include)
# define __has_include(x) 0
#endif
#if !defined(__has_attribute)
# define __has_attribute(x) 0
#endif
#if !defined(__has_feature)
# define __has_feature(x) 0
#endif
#if !defined(__has_warning)
# define __has_warning(x) 0
#endif

#if __has_include(<swift/objc-prologue.h>)
# include <swift/objc-prologue.h>
#endif

#pragma clang diagnostic ignored "-Wauto-import"
#include <objc/NSObject.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#if !defined(SWIFT_TYPEDEFS)
# define SWIFT_TYPEDEFS 1
# if __has_include(<uchar.h>)
#  include <uchar.h>
# elif !defined(__cplusplus)
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;
# endif
typedef float swift_float2  __attribute__((__ext_vector_type__(2)));
typedef float swift_float3  __attribute__((__ext_vector_type__(3)));
typedef float swift_float4  __attribute__((__ext_vector_type__(4)));
typedef double swift_double2  __attribute__((__ext_vector_type__(2)));
typedef double swift_double3  __attribute__((__ext_vector_type__(3)));
typedef double swift_double4  __attribute__((__ext_vector_type__(4)));
typedef int swift_int2  __attribute__((__ext_vector_type__(2)));
typedef int swift_int3  __attribute__((__ext_vector_type__(3)));
typedef int swift_int4  __attribute__((__ext_vector_type__(4)));
typedef unsigned int swift_uint2  __attribute__((__ext_vector_type__(2)));
typedef unsigned int swift_uint3  __attribute__((__ext_vector_type__(3)));
typedef unsigned int swift_uint4  __attribute__((__ext_vector_type__(4)));
#endif

#if !defined(SWIFT_PASTE)
# define SWIFT_PASTE_HELPER(x, y) x##y
# define SWIFT_PASTE(x, y) SWIFT_PASTE_HELPER(x, y)
#endif
#if !defined(SWIFT_METATYPE)
# define SWIFT_METATYPE(X) Class
#endif
#if !defined(SWIFT_CLASS_PROPERTY)
# if __has_feature(objc_class_property)
#  define SWIFT_CLASS_PROPERTY(...) __VA_ARGS__
# else
#  define SWIFT_CLASS_PROPERTY(...)
# endif
#endif

#if __has_attribute(objc_runtime_name)
# define SWIFT_RUNTIME_NAME(X) __attribute__((objc_runtime_name(X)))
#else
# define SWIFT_RUNTIME_NAME(X)
#endif
#if __has_attribute(swift_name)
# define SWIFT_COMPILE_NAME(X) __attribute__((swift_name(X)))
#else
# define SWIFT_COMPILE_NAME(X)
#endif
#if __has_attribute(objc_method_family)
# define SWIFT_METHOD_FAMILY(X) __attribute__((objc_method_family(X)))
#else
# define SWIFT_METHOD_FAMILY(X)
#endif
#if __has_attribute(noescape)
# define SWIFT_NOESCAPE __attribute__((noescape))
#else
# define SWIFT_NOESCAPE
#endif
#if __has_attribute(warn_unused_result)
# define SWIFT_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
# define SWIFT_WARN_UNUSED_RESULT
#endif
#if __has_attribute(noreturn)
# define SWIFT_NORETURN __attribute__((noreturn))
#else
# define SWIFT_NORETURN
#endif
#if !defined(SWIFT_CLASS_EXTRA)
# define SWIFT_CLASS_EXTRA
#endif
#if !defined(SWIFT_PROTOCOL_EXTRA)
# define SWIFT_PROTOCOL_EXTRA
#endif
#if !defined(SWIFT_ENUM_EXTRA)
# define SWIFT_ENUM_EXTRA
#endif
#if !defined(SWIFT_CLASS)
# if __has_attribute(objc_subclassing_restricted)
#  define SWIFT_CLASS(SWIFT_NAME) SWIFT_RUNTIME_NAME(SWIFT_NAME) __attribute__((objc_subclassing_restricted)) SWIFT_CLASS_EXTRA
#  define SWIFT_CLASS_NAMED(SWIFT_NAME) __attribute__((objc_subclassing_restricted)) SWIFT_COMPILE_NAME(SWIFT_NAME) SWIFT_CLASS_EXTRA
# else
#  define SWIFT_CLASS(SWIFT_NAME) SWIFT_RUNTIME_NAME(SWIFT_NAME) SWIFT_CLASS_EXTRA
#  define SWIFT_CLASS_NAMED(SWIFT_NAME) SWIFT_COMPILE_NAME(SWIFT_NAME) SWIFT_CLASS_EXTRA
# endif
#endif

#if !defined(SWIFT_PROTOCOL)
# define SWIFT_PROTOCOL(SWIFT_NAME) SWIFT_RUNTIME_NAME(SWIFT_NAME) SWIFT_PROTOCOL_EXTRA
# define SWIFT_PROTOCOL_NAMED(SWIFT_NAME) SWIFT_COMPILE_NAME(SWIFT_NAME) SWIFT_PROTOCOL_EXTRA
#endif

#if !defined(SWIFT_EXTENSION)
# define SWIFT_EXTENSION(M) SWIFT_PASTE(M##_Swift_, __LINE__)
#endif

#if !defined(OBJC_DESIGNATED_INITIALIZER)
# if __has_attribute(objc_designated_initializer)
#  define OBJC_DESIGNATED_INITIALIZER __attribute__((objc_designated_initializer))
# else
#  define OBJC_DESIGNATED_INITIALIZER
# endif
#endif
#if !defined(SWIFT_ENUM_ATTR)
# if defined(__has_attribute) && __has_attribute(enum_extensibility)
#  define SWIFT_ENUM_ATTR(_extensibility) __attribute__((enum_extensibility(_extensibility)))
# else
#  define SWIFT_ENUM_ATTR(_extensibility)
# endif
#endif
#if !defined(SWIFT_ENUM)
# define SWIFT_ENUM(_type, _name, _extensibility) enum _name : _type _name; enum SWIFT_ENUM_ATTR(_extensibility) SWIFT_ENUM_EXTRA _name : _type
# if __has_feature(generalized_swift_name)
#  define SWIFT_ENUM_NAMED(_type, _name, SWIFT_NAME, _extensibility) enum _name : _type _name SWIFT_COMPILE_NAME(SWIFT_NAME); enum SWIFT_COMPILE_NAME(SWIFT_NAME) SWIFT_ENUM_ATTR(_extensibility) SWIFT_ENUM_EXTRA _name : _type
# else
#  define SWIFT_ENUM_NAMED(_type, _name, SWIFT_NAME, _extensibility) SWIFT_ENUM(_type, _name, _extensibility)
# endif
#endif
#if !defined(SWIFT_UNAVAILABLE)
# define SWIFT_UNAVAILABLE __attribute__((unavailable))
#endif
#if !defined(SWIFT_UNAVAILABLE_MSG)
# define SWIFT_UNAVAILABLE_MSG(msg) __attribute__((unavailable(msg)))
#endif
#if !defined(SWIFT_AVAILABILITY)
# define SWIFT_AVAILABILITY(plat, ...) __attribute__((availability(plat, __VA_ARGS__)))
#endif
#if !defined(SWIFT_DEPRECATED)
# define SWIFT_DEPRECATED __attribute__((deprecated))
#endif
#if !defined(SWIFT_DEPRECATED_MSG)
# define SWIFT_DEPRECATED_MSG(...) __attribute__((deprecated(__VA_ARGS__)))
#endif
#if __has_feature(attribute_diagnose_if_objc)
# define SWIFT_DEPRECATED_OBJC(Msg) __attribute__((diagnose_if(1, Msg, "warning")))
#else
# define SWIFT_DEPRECATED_OBJC(Msg) SWIFT_DEPRECATED_MSG(Msg)
#endif
#if __has_feature(modules)
@import AppKit;
@import CoreGraphics;
@import Foundation;
#endif

#pragma clang diagnostic ignored "-Wproperty-attribute-mismatch"
#pragma clang diagnostic ignored "-Wduplicate-method-arg"
#if __has_warning("-Wpragma-clang-attribute")
# pragma clang diagnostic ignored "-Wpragma-clang-attribute"
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wnullability"

#if __has_attribute(external_source_symbol)
# pragma push_macro("any")
# undef any
# pragma clang attribute push(__attribute__((external_source_symbol(language="Swift", defined_in="KRActivityIndicatorView",generated_declaration))), apply_to=any(function,enum,objc_interface,objc_category,objc_protocol))
# pragma pop_macro("any")
#endif

@class NSColor;
@class NSFont;
@class NSCoder;

/// Activity indicator view with nice animations
SWIFT_CLASS("_TtC23KRActivityIndicatorView23KRActivityIndicatorView")
@interface KRActivityIndicatorView : NSView
/// Default color. Default value is NSColor.white.
SWIFT_CLASS_PROPERTY(@property (nonatomic, class, strong) NSColor * _Nonnull DEFAULT_COLOR SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_COLOR' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");)
+ (NSColor * _Nonnull)DEFAULT_COLOR SWIFT_WARN_UNUSED_RESULT SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_COLOR' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
+ (void)setDEFAULT_COLOR:(NSColor * _Nonnull)value SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_COLOR' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
/// Default padding. Default value is 0.0 - 0% padding
SWIFT_CLASS_PROPERTY(@property (nonatomic, class) CGFloat DEFAULT_PADDING SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_PADDING' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");)
+ (CGFloat)DEFAULT_PADDING SWIFT_WARN_UNUSED_RESULT SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_PADDING' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
+ (void)setDEFAULT_PADDING:(CGFloat)value SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_PADDING' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
/// Default size of activity indicator view in UI blocker. Default value is 60x60.
SWIFT_CLASS_PROPERTY(@property (nonatomic, class) CGSize DEFAULT_BLOCKER_SIZE SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_SIZE' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");)
+ (CGSize)DEFAULT_BLOCKER_SIZE SWIFT_WARN_UNUSED_RESULT SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_SIZE' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
+ (void)setDEFAULT_BLOCKER_SIZE:(CGSize)value SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_SIZE' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
/// Default display time threshold to actually display UI blocker. Default value is 0 ms.
SWIFT_CLASS_PROPERTY(@property (nonatomic, class) NSInteger DEFAULT_BLOCKER_DISPLAY_TIME_THRESHOLD SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_DISPLAY_TIME_THRESHOLD' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");)
+ (NSInteger)DEFAULT_BLOCKER_DISPLAY_TIME_THRESHOLD SWIFT_WARN_UNUSED_RESULT SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_DISPLAY_TIME_THRESHOLD' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
+ (void)setDEFAULT_BLOCKER_DISPLAY_TIME_THRESHOLD:(NSInteger)value SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_DISPLAY_TIME_THRESHOLD' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
/// Default minimum display time of UI blocker. Default value is 0 ms.
SWIFT_CLASS_PROPERTY(@property (nonatomic, class) NSInteger DEFAULT_BLOCKER_MINIMUM_DISPLAY_TIME SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_MINIMUM_DISPLAY_TIME' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");)
+ (NSInteger)DEFAULT_BLOCKER_MINIMUM_DISPLAY_TIME SWIFT_WARN_UNUSED_RESULT SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_MINIMUM_DISPLAY_TIME' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
+ (void)setDEFAULT_BLOCKER_MINIMUM_DISPLAY_TIME:(NSInteger)value SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_MINIMUM_DISPLAY_TIME' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
/// Default message displayed in UI blocker. Default value is nil.
SWIFT_CLASS_PROPERTY(@property (nonatomic, class, copy) NSString * _Nullable DEFAULT_BLOCKER_MESSAGE SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_MESSAGE' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");)
+ (NSString * _Nullable)DEFAULT_BLOCKER_MESSAGE SWIFT_WARN_UNUSED_RESULT SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_MESSAGE' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
+ (void)setDEFAULT_BLOCKER_MESSAGE:(NSString * _Nullable)value SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_MESSAGE' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
/// Default font of message displayed in UI blocker. Default value is bold system font, size 20.
SWIFT_CLASS_PROPERTY(@property (nonatomic, class, strong) NSFont * _Nonnull DEFAULT_BLOCKER_MESSAGE_FONT SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_MESSAGE_FONT' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");)
+ (NSFont * _Nonnull)DEFAULT_BLOCKER_MESSAGE_FONT SWIFT_WARN_UNUSED_RESULT SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_MESSAGE_FONT' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
+ (void)setDEFAULT_BLOCKER_MESSAGE_FONT:(NSFont * _Nonnull)value SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_MESSAGE_FONT' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
/// Default background color of UI blocker. Default value is UIColor(red: 0, green: 0, blue: 0, alpha: 0.5)
SWIFT_CLASS_PROPERTY(@property (nonatomic, class, strong) NSColor * _Nonnull DEFAULT_BLOCKER_BACKGROUND_COLOR SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_BACKGROUND_COLOR' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");)
+ (NSColor * _Nonnull)DEFAULT_BLOCKER_BACKGROUND_COLOR SWIFT_WARN_UNUSED_RESULT SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_BACKGROUND_COLOR' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
+ (void)setDEFAULT_BLOCKER_BACKGROUND_COLOR:(NSColor * _Nonnull)value SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.DEFAULT_BLOCKER_BACKGROUND_COLOR' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
/// Color of activity indicator view.
@property (nonatomic, strong) NSColor * _Nonnull color;
/// Padding of activity indicator view.
@property (nonatomic) CGFloat padding;
/// Current status of animation, read-only.
@property (nonatomic, readonly) BOOL animating SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.animating' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
/// Current status of animation, read-only.
@property (nonatomic, readonly) BOOL isAnimating SWIFT_DEPRECATED_OBJC("Swift property 'KRActivityIndicatorView.isAnimating' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
/// Returns an object initialized from data in a given unarchiver.
/// self, initialized using the data in decoder.
/// \param decoder an unarchiver object.
///
///
/// returns:
/// self, initialized using the data in decoder.
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)aDecoder OBJC_DESIGNATED_INITIALIZER;
/// Returns the natural size for the receiving view, considering only properties of the view itself.
/// A size indicating the natural size for the receiving view based on its intrinsic properties.
///
/// returns:
/// A size indicating the natural size for the receiving view based on its intrinsic properties.
@property (nonatomic, readonly) CGSize intrinsicContentSize;
/// Start animating.
- (void)startAnimating SWIFT_DEPRECATED_OBJC("Swift method 'KRActivityIndicatorView.startAnimating()' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
/// Stop animating.
- (void)stopAnimating SWIFT_DEPRECATED_OBJC("Swift method 'KRActivityIndicatorView.stopAnimating()' uses '@objc' inference deprecated in Swift 4; add '@objc' to provide an Objective-C entrypoint");
- (nonnull instancetype)initWithFrame:(NSRect)frameRect SWIFT_UNAVAILABLE;
@end

#if __has_attribute(external_source_symbol)
# pragma clang attribute pop
#endif
#pragma clang diagnostic pop
