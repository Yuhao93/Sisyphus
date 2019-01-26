package haodev.com.sisyphus;

import java.util.List;

import sisyphus.Model;

/**
 * Created by yuhao on 9/3/2018.
 */

public interface PatternInterpolator {
  boolean accepts(Model.StoredPattern pattern);
  List<Model.CartesianCoordinate> interpolate(Model.StoredPattern pattern);
}
