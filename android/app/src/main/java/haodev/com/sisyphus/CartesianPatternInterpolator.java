package haodev.com.sisyphus;

import java.util.List;

import sisyphus.Model;

/**
 * Created by yuhao on 9/3/2018.
 */

public class CartesianPatternInterpolator implements PatternInterpolator {
  @Override
  public boolean accepts(Model.StoredPattern pattern) {
    return pattern.hasCartesian();
  }

  @Override
  public List<Model.CartesianCoordinate> interpolate(Model.StoredPattern pattern) {
    return pattern.getCartesian().getCoordinateList();
  }
}
