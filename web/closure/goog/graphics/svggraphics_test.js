// Copyright 2011 The Closure Library Authors. All Rights Reserved.
// Use of this source code is governed by the Apache License, Version 2.0.

goog.provide('goog.graphics.SvgGraphicsTest');
goog.setTestOnly('goog.graphics.SvgGraphicsTest');

goog.require('goog.dom');
goog.require('goog.graphics.AffineTransform');
goog.require('goog.graphics.SolidFill');
goog.require('goog.graphics.SvgGraphics');
goog.require('goog.testing.jsunit');

var graphics;

function setUp() {
  if (!document.createElementNS) {
    // Some browsers don't support document.createElementNS and this test
    // should not be run on those browsers (IE7,8).
    return;
  }
  graphics = new goog.graphics.SvgGraphics('100px', '100px');
  graphics.createDom();
  goog.dom.getElement('root').appendChild(graphics.getElement());
}

function testAddDef() {
  if (!graphics) {
    // setUp has failed (no browser support), we should not run this test.
    return;
  }
  var defElement1 = document.createElement('div');
  var defElement2 = document.createElement('div');
  var defKey1 = 'def1';
  var defKey2 = 'def2';
  var id = graphics.addDef(defKey1, defElement1);
  assertEquals('_svgdef_0', id);
  id = graphics.addDef(defKey1, defElement2);
  assertEquals('_svgdef_0', id);
  id = graphics.addDef(defKey2, defElement2);
  assertEquals('_svgdef_1', id);
}

function testGetDef() {
  if (!graphics) {
    // setUp has failed (no browser support), we should not run this test.
    return;
  }
  var defElement = document.createElement('div');
  var defKey = 'def';
  var id = graphics.addDef(defKey, defElement);
  assertEquals(id, graphics.getDef(defKey));
  assertNull(graphics.getDef('randomKey'));
}

function testRemoveDef() {
  if (!graphics) {
    // setUp has failed (no browser support), we should not run this test.
    return;
  }
  var defElement = document.createElement('div');
  var defKey = 'def';
  var addedId = graphics.addDef(defKey, defElement);
  graphics.removeDef('randomKey');
  assertEquals(addedId, graphics.getDef(defKey));
  graphics.removeDef(defKey);
  assertNull(graphics.getDef(defKey));
}

function testSetElementAffineTransform() {
  if (!graphics) {
    // setUp has failed (no browser support), we should not run this test.
    return;
  }
  var fill = new goog.graphics.SolidFill('blue');
  var stroke = null;
  var rad = -3.1415926 / 6;
  var costheta = Math.cos(rad);
  var sintheta = Math.sin(rad);
  var dx = 10;
  var dy = -20;
  var affine = new goog.graphics.AffineTransform(
    costheta, -sintheta + 1, sintheta, costheta, dx, dy);
  var rect = graphics.drawRect(10, 20, 30, 40, stroke, fill);
  rect.setTransform(affine);
  graphics.render();
  // getTransformToElement was deleted in Chrome48. See
  // https://code.google.com/p/chromium/issues/detail?id=524432.
  function getTransformToElement(element, target) {
    return target.getScreenCTM().inverse().multiply(element.getScreenCTM());
  }
  var svgMatrix =
      getTransformToElement(rect.getElement(), graphics.getElement());
  assertRoughlyEquals(svgMatrix.a, costheta, 0.001);
  assertRoughlyEquals(svgMatrix.b, -sintheta + 1, 0.001);
  assertRoughlyEquals(svgMatrix.c, sintheta, 0.001);
  assertRoughlyEquals(svgMatrix.d, costheta, 0.001);
  assertRoughlyEquals(svgMatrix.e, dx, 0.001);
  assertRoughlyEquals(svgMatrix.f, dy, 0.001);
}
