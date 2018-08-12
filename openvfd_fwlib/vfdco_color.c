/*
 * Being part of something special makes you special
 *
 * Created summer 2018
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

 static float _vfdco_color_hue2rgb(float p, float q, float t){
   if(t < 0.0) t += 1;
   if(t > 1.0) t -= 1;
   if(t < 0.16666666667f) return p + (q - p) * 6.0f * t;
   if(t < 0.5f) return q;
   if(t < 0.66666666667f) return p + (q - p) * (0.66666666667f - t) * 6.0f;
   return p;
 }

uint32_t vfdco_color_hsl2rgb(uint8_t hue, uint8_t sat, uint8_t lum) {
  // https://stackoverflow.com/questions/2353211
  float h = hue / 255.0f;
  float s = sat / 255.0f;
  float l = lum / 255.0f;
  float r, g, b;

  if(sat == 0){
      r = l; g = l; b = l;
  } else{
      float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
      float p = 2.0f * l - q;
      r = _vfdco_color_hue2rgb(p, q, h + 0.333333333333f);
      g = _vfdco_color_hue2rgb(p, q, h);
      b = _vfdco_color_hue2rgb(p, q, h - 0.333333333333f);
  }
  return ((uint32_t)(255.0f * g) << 16) | ((uint32_t)(255.0f * r) << 8) | (uint32_t)(255.0f * b));
}
