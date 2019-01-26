package haodev.com.sisyphus;

import android.util.Pair;

import java.util.List;
import java.util.stream.Collectors;

import sisyphus.Model;

/**
 * Created by yuhao on 9/3/2018.
 */

public class CartesianParametricPatternInterpolator extends ParametricPatternInterpolator{
  @Override
  boolean accepts(Model.ParametricPattern pattern) {
    return pattern.getType() == Model.ParametricPattern.Type.CARTESIAN;
  }

  @Override
  List<Model.CartesianCoordinate> interpolate(List<Pair<Double, Double>> pattern) {
    return pattern.stream().map(this::toCartesianCoordinate).collect(Collectors.toList());
  }

  private Model.CartesianCoordinate toCartesianCoordinate(Pair<Double, Double> point) {
    return Model.CartesianCoordinate.newBuilder()
        .setX(point.first.floatValue())
        .setY(point.second.floatValue())
        .build();
  }
}
