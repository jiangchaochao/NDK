package com.jiangc.mqttndk;

import android.content.Context;
import android.content.res.AssetManager;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class Utils {
    private Utils() {
    }

    private static class SingletonHolder {
        private static final Utils INSTANCE = new Utils();
    }

    public static Utils getInstance() {
        return SingletonHolder.INSTANCE;
    }


    /**
     * 获取Assets文件的绝对地址，其实就是将文件拷贝到应用内的目录，然后获取绝对地址
     *
     * @param context 上下文
     * @param file    文件名
     * @return 文件绝对地址
     */
    public String getPath(Context context, String file) {
        AssetManager assetManager = context.getAssets();
        BufferedInputStream inputStream = null;
        try {
            // 打开文件放入输入流中
            inputStream = new BufferedInputStream(assetManager.open(file));
            // 输入流信息防盗data中
            byte[] data = new byte[inputStream.available()];
            // 输入流读取data
            inputStream.read(data);
            // 关闭输入流
            inputStream.close();
            // 创建一个file，放到一个缓存目录
            File outFile = new File(context.getFilesDir(), file);
            // 创建一个输出流
            FileOutputStream os = new FileOutputStream(outFile);
            // 将读取的数据放到输出流中
            os.write(data);
            os.close();
            return outFile.getAbsolutePath();  // 返回文件绝对地址
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
