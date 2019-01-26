package haodev.com.sisyphus;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.View;

import sisyphus.Model;

/**
 * Created by yuhao on 9/4/2018.
 */

public class PatternView extends View {
  private PatternRenderer renderer = new PatternRenderer();
  private Model.StoredPattern storedPattern;

  private static final float DEG_PER_MILLIS = .005f;
  private static final float RADIUS_PER_MILLIS = .015f;


  public PatternView(Context context, @Nullable AttributeSet attrs) {
    super(context, attrs);
    storedPattern =
        Model.StoredPattern.newBuilder()
            .setParametric(
                Model.ParametricPattern.newBuilder()
                    .setEquationForXOrTheta(
                        Model.ParametricPattern.Equation.newBuilder()
                            .setBinaryEquation(
                                Model.ParametricPattern.Equation.BinaryEquation.newBuilder()
                                    .setOpertation(Model.ParametricPattern.Equation.BinaryEquation.BinaryOperationType.MULTIPLY)
                                    .setEquationLeft(Model.ParametricPattern.Equation.newBuilder().setVarible(Model.ParametricPattern.Equation.Variable.T))
                                    .setEquationRight(Model.ParametricPattern.Equation.newBuilder().setConstant(DEG_PER_MILLIS))))
                    .setEquationForYOrR(
                        Model.ParametricPattern.Equation.newBuilder()
                            .setBinaryEquation(
                                Model.ParametricPattern.Equation.BinaryEquation.newBuilder()
                                    .setOpertation(Model.ParametricPattern.Equation.BinaryEquation.BinaryOperationType.MULTIPLY)
                                    .setEquationLeft(Model.ParametricPattern.Equation.newBuilder().setVarible(Model.ParametricPattern.Equation.Variable.T))
                                    .setEquationRight(Model.ParametricPattern.Equation.newBuilder().setConstant(RADIUS_PER_MILLIS))))
                    .setType(Model.ParametricPattern.Type.POLAR)
                    .setStartT(0)
                    .setEndT(50000)
                    .setTRateMillis(1))
            .build();


  }

  @Override
  public void onDraw(Canvas canvas) {
    canvas.drawColor(Color.WHITE);
    canvas.save();
    canvas.translate(canvas.getWidth() / 2, canvas.getHeight() / 2);
    System.out.println("A");
    renderer.render(canvas, storedPattern);
    System.out.println("B");
    canvas.restore();
  }
}
