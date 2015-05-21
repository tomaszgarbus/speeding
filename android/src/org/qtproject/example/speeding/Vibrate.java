//
// Vibrate.java
//
package org.qtproject.example.speeding;

import android.content.Context;
import android.os.Vibrator;
import android.app.Activity;
import android.os.Bundle;
import android.view.WindowManager;

public class Vibrate extends org.qtproject.qt5.android.bindings.QtActivity
{

   public static Vibrator m_vibrator;
   public static Vibrate m_istance;

   public Vibrate()
   {
       m_istance = this;

       //getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
   }

   public static void start(int x)
   {
       if (m_vibrator == null)
       {
           if (m_istance != null)
           {
             m_vibrator = (Vibrator)  m_istance.getSystemService(Context.VIBRATOR_SERVICE);
             m_vibrator.vibrate(x);
           }

       }
       else m_vibrator.vibrate(x);

   }
}
