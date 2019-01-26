package haodev.com.sisyphus;

import java.util.List;
import java.util.stream.Collectors;

import sisyphus.Model;

/**
 * Created by yuhao on 9/3/2018.
 */

public class PolarPatternInterpolator implements PatternInterpolator {
  @Override
  public boolean accepts(Model.StoredPattern pattern) {
    return pattern.hasPolar();
  }

  @Override
  public List<Model.CartesianCoordinate> interpolate(Model.StoredPattern pattern) {
    return pattern.getPolar().getCoordinateList().stream()
        .map(this::toCartesian)
        .collect(Collectors.toList());
  }

  private Model.CartesianCoordinate toCartesian(Model.PolarCoordinate polarCoordinate) {
    return Model.CartesianCoordinate.newBuilder()
        .setX((float) Math.cos(polarCoordinate.getA()) * polarCoordinate.getR())
        .setY((float) Math.sin(polarCoordinate.getA()) * polarCoordinate.getR())
        .build();
  }
}
