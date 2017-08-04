#ifndef OPENSLES_SRSLABS_H

#ifdef __cplusplus
extern "C" {
#endif 

/*---------------------------------------------------------------------------------*/
/* SRS Labs Configuration Keys                                                     */
/* Use these key/values for SLAndroidConfigurationItf and SLConfigurationExtension */
/*---------------------------------------------------------------------------------*/
#if defined(WIN32) || defined(_WIN32)
    /*
     * Use this key to set the window handle for DSound.
     * Key:     SL_SRSLABS_CONFIGKEY_WINDOW_HANDLE
     * Value:   Address of the window handle.
     * Size:    sizeof(HWND)
     */
    #define SL_SRSLABS_CONFIGKEY_WINDOW_HANDLE ((const SLchar*) "SRS_CONFIG_KEY_WINDOWHANDLE")
#endif


#ifdef __cplusplus
}
#endif 

#endif // OPENSLES_SRSLABS_H 