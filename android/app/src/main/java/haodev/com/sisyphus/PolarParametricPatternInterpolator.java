package haodev.com.sisyphus;

import android.util.Pair;

import java.util.List;
import java.util.stream.Collectors;

import sisyphus.Model;

/**
 * Created by yuhao on 9/3/2018.
 */

public class PolarParametricPatternInterpolator extends ParametricPatternInterpolator {
  @Override
  boolean accepts(Model.ParametricPattern pattern) {
    return pattern.getType() == Model.ParametricPattern.Type.POLAR;
  }

  @Override
  List<Model.CartesianCoordinate> interpolate(List<Pair<Double, Double>> pattern) {
    return pattern.stream().map(this::toCartesian).collect(Collectors.toList());
  }

  private Model.CartesianCoordinate toCartesian(Pair<Double, Double> point) {
    return Model.CartesianCoordinate.newBuilder()
        .setX((float) Math.cos(point.first) * point.second.floatValue())
        .setY((float) Math.sin(point.first) * point.second.floatValue())
        .build();
  }
}
