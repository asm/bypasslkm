package bypasslkm.bypasslkm;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // I suppose it would be nice to log something to screen :)
        bypasslkm(false);

        Log.i("bypasslkm", "Finished patching!");
    }

    public native long bypasslkm(boolean restore);

    static {
        System.loadLibrary("bypasslkm");
    }
}
