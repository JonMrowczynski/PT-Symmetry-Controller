/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class ptsymmetrycontroller_hardware_LabQuest2 */

#ifndef _Included_ptsymmetrycontroller_hardware_LabQuest2
#define _Included_ptsymmetrycontroller_hardware_LabQuest2
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     ptsymmetrycontroller_hardware_LabQuest2
 * Method:    initLabQuest2AndPhotogates
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_ptsymmetrycontroller_hardware_LabQuest2_initLabQuest2AndPhotogates
  (JNIEnv *, jobject);

/*
 * Class:     ptsymmetrycontroller_hardware_LabQuest2
 * Method:    closeLabQuest2AndPhotogates
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ptsymmetrycontroller_hardware_LabQuest2_closeLabQuest2AndPhotogates
  (JNIEnv *, jobject);

/*
 * Class:     ptsymmetrycontroller_hardware_LabQuest2
 * Method:    pollDrivingPhotogate
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ptsymmetrycontroller_hardware_LabQuest2_pollDrivingPhotogate
  (JNIEnv *, jobject);

/*
 * Class:     ptsymmetrycontroller_hardware_LabQuest2
 * Method:    pollDampeningPhotogate
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ptsymmetrycontroller_hardware_LabQuest2_pollDampeningPhotogate
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
