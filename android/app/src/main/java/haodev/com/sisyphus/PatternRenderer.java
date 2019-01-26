package haodev.com.sisyphus;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;

import java.util.ArrayList;
import java.util.List;

import sisyphus.Model;
import sisyphus.Model.StoredPattern;

public class PatternRenderer {
  private static final Paint TRAVERSED = new Paint();
  private static final Paint PROJECTED = new Paint();

  private List<PatternInterpolator> interpolators = new ArrayList<>();

  public PatternRenderer() {
    interpolators.add(new CartesianPatternInterpolator());
    interpolators.add(new PolarPatternInterpolator());
    interpolators.add(new CartesianParametricPatternInterpolator());
    interpolators.add(new PolarParametricPatternInterpolator());
    TRAVERSED.setColor(Color.BLACK);
    PROJECTED.setColor(Color.BLACK);
    TRAVERSED.setStrokeWidth(1);
    TRAVERSED.setAntiAlias(true);
  }

  public void render(Canvas canvas, StoredPattern storedPattern) {
    for (PatternInterpolator interpolator : interpolators) {
      if (interpolator.accepts(storedPattern)) {
        List<Model.CartesianCoordinate> points = interpolator.interpolate(storedPattern);
        for (int i = 1; i < points.size(); i++) {
          Model.CartesianCoordinate start = points.get(i - 1);
          Model.CartesianCoordinate end = points.get(i);
          canvas.drawLine(start.getX(), start.getY(), end.getX(), end.getY(), TRAVERSED);
        }
        return;
      }
    }
    throw new IllegalArgumentException("No interpolator can accept stored pattern");
  }
}
