package info.skorka.chris.legorobotcontroller;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;

public class SliderView extends View {

    public SliderView(Context context, AttributeSet attrs){
        super(context, attrs);
    }

    private int trackWidth = 30;
    private int thumbWidth = 50;
    private int thumbHeight = 20;


    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        System.out.println("Canvas: ");
        System.out.println(canvas.getClipBounds().toString());

        int height = canvas.getHeight();

        float radius = 5.0f;
        int trackLeft = (thumbWidth - trackWidth) / 2;
        int trackRight = trackLeft + trackWidth;
        int trackTop = thumbHeight / 2;
        int trackBottom = height - trackTop;
        Paint trackPaint = new Paint();
        trackPaint.setColor(0xFF0000);

        canvas.drawRoundRect(trackLeft, trackTop, trackRight, -trackBottom, radius, radius, trackPaint);
    }

    // return default width of view
    @Override
    protected int getSuggestedMinimumWidth() {
        return 100;

        // return super.getSuggestedMinimumWidth();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }

}
