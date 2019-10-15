#include "watertank.hh"

watertank::watertank(sc_module_name) :
    aperture_ams("aperture_ams"),
    input_water_ams("input_water_ams"),
    converter_in("converter_in"),
    converter_out("converter_out"),
    integ("integ"),
    gain1("gain1", 0.6),
    gain2("gain2", 0.03),
    sub("sub"),
    sig1("sig1"),
    sig2("sig2"),
    sig3("sig3"),
    sig4("sig4"),
    sig5("sig5") {

  converter_in.inp(aperture_ams); // read the current value of the aperture
  converter_in.y(sig1); // put result on sig1

  // 0.6 * a
  gain1.x(sig1);
  gain1.y(sig2);

  // x'
  sub.x1(sig2);
  sub.x2(sig5);
  sub.y(sig3);

  // x
  integ.x(sig3);
  integ.y(sig4);

  // 0.03 * x
  gain2.x(sig4);
  gain2.y(sig5);

  converter_out.x(sig4);
  converter_out.outp(input_water_ams);

}
