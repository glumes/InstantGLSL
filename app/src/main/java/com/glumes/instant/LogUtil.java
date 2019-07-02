package com.glumes.instant;

import android.util.Log;


public class LogUtil {

    private static String DEFAULT_TAG = "instantglsl";

    private static String DEBUG = "mydebug";
    private static boolean mLogAll = false; // 默认关闭log输出
    private static final boolean LOGD_DEBUG = true;
    private static final boolean LOGI_DEBUG = true;
    private static final boolean LOGE_DEBUG = true;
    private static final boolean LOGDB_DEBUG = false;
    private static final int STACK_INDEX = 2;


    private LogUtil() {
    }

    /**
     * Used for persistent log
     * @param msg
     */
    public static void d(String msg) {
        if (LOGD_DEBUG && mLogAll) {
            Log.d(DEFAULT_TAG, getInformation(msg));
        }
    }

    /**
     * Used for temp log
     * @param msg
     */
    public static void i(String msg) {
        if (LOGI_DEBUG && mLogAll) {
            Log.i(DEFAULT_TAG, getInformation(msg));
        }
    }

    /**
     * Used for exception log
     * @param msg
     */
    public static void e(String msg) {
        if (LOGE_DEBUG && mLogAll) {
            Log.e(DEFAULT_TAG, getInformation(msg));
        }
    }


    /**
     * Used for exception log
     * @param msg
     * @param t
     */
    public static void e(String msg, Throwable t) {
        if (LOGE_DEBUG && mLogAll) {
            Log.e(DEFAULT_TAG, getInformation(msg), t);
        }
    }

    private static String getInformation(String msg) {
        Exception exception = new Exception();
        return exception.getStackTrace()[STACK_INDEX].getFileName() + "|"
                + exception.getStackTrace()[STACK_INDEX].getMethodName() + "|"
                + exception.getStackTrace()[STACK_INDEX].getLineNumber() + "|" + msg;
    }

    public static void setLog(boolean enable) {
        mLogAll = enable;
    }

    public static boolean isLog() { return mLogAll ;}
}
