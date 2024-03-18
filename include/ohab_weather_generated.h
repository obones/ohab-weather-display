// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_OHABWEATHER_OHAB_WEATHER_H_
#define FLATBUFFERS_GENERATED_OHABWEATHER_OHAB_WEATHER_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 7,
             "Non-compatible flatbuffers version included");

namespace ohab_weather {

struct Weather;
struct WeatherBuilder;

struct CurrentWeather;
struct CurrentWeatherBuilder;

struct Forecast;
struct ForecastBuilder;

struct Alerts;
struct AlertsBuilder;

struct Weather FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef WeatherBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CURRENT = 4,
    VT_DAYS = 6,
    VT_ALERTS = 8,
    VT_WINDSPEEDUNIT = 10,
    VT_PRECIPITATIONUNIT = 12
  };
  const ohab_weather::CurrentWeather *current() const {
    return GetPointer<const ohab_weather::CurrentWeather *>(VT_CURRENT);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<ohab_weather::Forecast>> *days() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<ohab_weather::Forecast>> *>(VT_DAYS);
  }
  const ohab_weather::Alerts *alerts() const {
    return GetPointer<const ohab_weather::Alerts *>(VT_ALERTS);
  }
  const ::flatbuffers::String *windSpeedUnit() const {
    return GetPointer<const ::flatbuffers::String *>(VT_WINDSPEEDUNIT);
  }
  const ::flatbuffers::String *precipitationUnit() const {
    return GetPointer<const ::flatbuffers::String *>(VT_PRECIPITATIONUNIT);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_CURRENT) &&
           verifier.VerifyTable(current()) &&
           VerifyOffset(verifier, VT_DAYS) &&
           verifier.VerifyVector(days()) &&
           verifier.VerifyVectorOfTables(days()) &&
           VerifyOffset(verifier, VT_ALERTS) &&
           verifier.VerifyTable(alerts()) &&
           VerifyOffset(verifier, VT_WINDSPEEDUNIT) &&
           verifier.VerifyString(windSpeedUnit()) &&
           VerifyOffset(verifier, VT_PRECIPITATIONUNIT) &&
           verifier.VerifyString(precipitationUnit()) &&
           verifier.EndTable();
  }
};

struct WeatherBuilder {
  typedef Weather Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_current(::flatbuffers::Offset<ohab_weather::CurrentWeather> current) {
    fbb_.AddOffset(Weather::VT_CURRENT, current);
  }
  void add_days(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<ohab_weather::Forecast>>> days) {
    fbb_.AddOffset(Weather::VT_DAYS, days);
  }
  void add_alerts(::flatbuffers::Offset<ohab_weather::Alerts> alerts) {
    fbb_.AddOffset(Weather::VT_ALERTS, alerts);
  }
  void add_windSpeedUnit(::flatbuffers::Offset<::flatbuffers::String> windSpeedUnit) {
    fbb_.AddOffset(Weather::VT_WINDSPEEDUNIT, windSpeedUnit);
  }
  void add_precipitationUnit(::flatbuffers::Offset<::flatbuffers::String> precipitationUnit) {
    fbb_.AddOffset(Weather::VT_PRECIPITATIONUNIT, precipitationUnit);
  }
  explicit WeatherBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Weather> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Weather>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Weather> CreateWeather(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<ohab_weather::CurrentWeather> current = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<ohab_weather::Forecast>>> days = 0,
    ::flatbuffers::Offset<ohab_weather::Alerts> alerts = 0,
    ::flatbuffers::Offset<::flatbuffers::String> windSpeedUnit = 0,
    ::flatbuffers::Offset<::flatbuffers::String> precipitationUnit = 0) {
  WeatherBuilder builder_(_fbb);
  builder_.add_precipitationUnit(precipitationUnit);
  builder_.add_windSpeedUnit(windSpeedUnit);
  builder_.add_alerts(alerts);
  builder_.add_days(days);
  builder_.add_current(current);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Weather> CreateWeatherDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<ohab_weather::CurrentWeather> current = 0,
    const std::vector<::flatbuffers::Offset<ohab_weather::Forecast>> *days = nullptr,
    ::flatbuffers::Offset<ohab_weather::Alerts> alerts = 0,
    const char *windSpeedUnit = nullptr,
    const char *precipitationUnit = nullptr) {
  auto days__ = days ? _fbb.CreateVector<::flatbuffers::Offset<ohab_weather::Forecast>>(*days) : 0;
  auto windSpeedUnit__ = windSpeedUnit ? _fbb.CreateString(windSpeedUnit) : 0;
  auto precipitationUnit__ = precipitationUnit ? _fbb.CreateString(precipitationUnit) : 0;
  return ohab_weather::CreateWeather(
      _fbb,
      current,
      days__,
      alerts,
      windSpeedUnit__,
      precipitationUnit__);
}

struct CurrentWeather FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef CurrentWeatherBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_OUTDOORTEMPERATURE = 4,
    VT_WINDDIRECTION = 6,
    VT_WINDSPEED = 8,
    VT_RELATIVEHUMIDITY = 10,
    VT_MOONPHASE = 12
  };
  float outdoorTemperature() const {
    return GetField<float>(VT_OUTDOORTEMPERATURE, 0.0f);
  }
  float windDirection() const {
    return GetField<float>(VT_WINDDIRECTION, 0.0f);
  }
  float windSpeed() const {
    return GetField<float>(VT_WINDSPEED, 0.0f);
  }
  float relativeHumidity() const {
    return GetField<float>(VT_RELATIVEHUMIDITY, 0.0f);
  }
  float moonPhase() const {
    return GetField<float>(VT_MOONPHASE, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_OUTDOORTEMPERATURE, 4) &&
           VerifyField<float>(verifier, VT_WINDDIRECTION, 4) &&
           VerifyField<float>(verifier, VT_WINDSPEED, 4) &&
           VerifyField<float>(verifier, VT_RELATIVEHUMIDITY, 4) &&
           VerifyField<float>(verifier, VT_MOONPHASE, 4) &&
           verifier.EndTable();
  }
};

struct CurrentWeatherBuilder {
  typedef CurrentWeather Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_outdoorTemperature(float outdoorTemperature) {
    fbb_.AddElement<float>(CurrentWeather::VT_OUTDOORTEMPERATURE, outdoorTemperature, 0.0f);
  }
  void add_windDirection(float windDirection) {
    fbb_.AddElement<float>(CurrentWeather::VT_WINDDIRECTION, windDirection, 0.0f);
  }
  void add_windSpeed(float windSpeed) {
    fbb_.AddElement<float>(CurrentWeather::VT_WINDSPEED, windSpeed, 0.0f);
  }
  void add_relativeHumidity(float relativeHumidity) {
    fbb_.AddElement<float>(CurrentWeather::VT_RELATIVEHUMIDITY, relativeHumidity, 0.0f);
  }
  void add_moonPhase(float moonPhase) {
    fbb_.AddElement<float>(CurrentWeather::VT_MOONPHASE, moonPhase, 0.0f);
  }
  explicit CurrentWeatherBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<CurrentWeather> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<CurrentWeather>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<CurrentWeather> CreateCurrentWeather(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float outdoorTemperature = 0.0f,
    float windDirection = 0.0f,
    float windSpeed = 0.0f,
    float relativeHumidity = 0.0f,
    float moonPhase = 0.0f) {
  CurrentWeatherBuilder builder_(_fbb);
  builder_.add_moonPhase(moonPhase);
  builder_.add_relativeHumidity(relativeHumidity);
  builder_.add_windSpeed(windSpeed);
  builder_.add_windDirection(windDirection);
  builder_.add_outdoorTemperature(outdoorTemperature);
  return builder_.Finish();
}

struct Forecast FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef ForecastBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MINTEMPERATURE = 4,
    VT_MAXTEMPERATURE = 6,
    VT_MAXWINDSPEED = 8,
    VT_DOMINANTWINDDIRECTION = 10,
    VT_CONDITIONCODE = 12,
    VT_CLOUDINESS = 14,
    VT_SNOW = 16,
    VT_RAIN = 18,
    VT_SHOWERS = 20
  };
  float minTemperature() const {
    return GetField<float>(VT_MINTEMPERATURE, 0.0f);
  }
  float maxTemperature() const {
    return GetField<float>(VT_MAXTEMPERATURE, 0.0f);
  }
  float maxWindSpeed() const {
    return GetField<float>(VT_MAXWINDSPEED, 0.0f);
  }
  float dominantWindDirection() const {
    return GetField<float>(VT_DOMINANTWINDDIRECTION, 0.0f);
  }
  uint8_t conditionCode() const {
    return GetField<uint8_t>(VT_CONDITIONCODE, 0);
  }
  float cloudiness() const {
    return GetField<float>(VT_CLOUDINESS, 0.0f);
  }
  float snow() const {
    return GetField<float>(VT_SNOW, 0.0f);
  }
  float rain() const {
    return GetField<float>(VT_RAIN, 0.0f);
  }
  float showers() const {
    return GetField<float>(VT_SHOWERS, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_MINTEMPERATURE, 4) &&
           VerifyField<float>(verifier, VT_MAXTEMPERATURE, 4) &&
           VerifyField<float>(verifier, VT_MAXWINDSPEED, 4) &&
           VerifyField<float>(verifier, VT_DOMINANTWINDDIRECTION, 4) &&
           VerifyField<uint8_t>(verifier, VT_CONDITIONCODE, 1) &&
           VerifyField<float>(verifier, VT_CLOUDINESS, 4) &&
           VerifyField<float>(verifier, VT_SNOW, 4) &&
           VerifyField<float>(verifier, VT_RAIN, 4) &&
           VerifyField<float>(verifier, VT_SHOWERS, 4) &&
           verifier.EndTable();
  }
};

struct ForecastBuilder {
  typedef Forecast Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_minTemperature(float minTemperature) {
    fbb_.AddElement<float>(Forecast::VT_MINTEMPERATURE, minTemperature, 0.0f);
  }
  void add_maxTemperature(float maxTemperature) {
    fbb_.AddElement<float>(Forecast::VT_MAXTEMPERATURE, maxTemperature, 0.0f);
  }
  void add_maxWindSpeed(float maxWindSpeed) {
    fbb_.AddElement<float>(Forecast::VT_MAXWINDSPEED, maxWindSpeed, 0.0f);
  }
  void add_dominantWindDirection(float dominantWindDirection) {
    fbb_.AddElement<float>(Forecast::VT_DOMINANTWINDDIRECTION, dominantWindDirection, 0.0f);
  }
  void add_conditionCode(uint8_t conditionCode) {
    fbb_.AddElement<uint8_t>(Forecast::VT_CONDITIONCODE, conditionCode, 0);
  }
  void add_cloudiness(float cloudiness) {
    fbb_.AddElement<float>(Forecast::VT_CLOUDINESS, cloudiness, 0.0f);
  }
  void add_snow(float snow) {
    fbb_.AddElement<float>(Forecast::VT_SNOW, snow, 0.0f);
  }
  void add_rain(float rain) {
    fbb_.AddElement<float>(Forecast::VT_RAIN, rain, 0.0f);
  }
  void add_showers(float showers) {
    fbb_.AddElement<float>(Forecast::VT_SHOWERS, showers, 0.0f);
  }
  explicit ForecastBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Forecast> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Forecast>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Forecast> CreateForecast(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float minTemperature = 0.0f,
    float maxTemperature = 0.0f,
    float maxWindSpeed = 0.0f,
    float dominantWindDirection = 0.0f,
    uint8_t conditionCode = 0,
    float cloudiness = 0.0f,
    float snow = 0.0f,
    float rain = 0.0f,
    float showers = 0.0f) {
  ForecastBuilder builder_(_fbb);
  builder_.add_showers(showers);
  builder_.add_rain(rain);
  builder_.add_snow(snow);
  builder_.add_cloudiness(cloudiness);
  builder_.add_dominantWindDirection(dominantWindDirection);
  builder_.add_maxWindSpeed(maxWindSpeed);
  builder_.add_maxTemperature(maxTemperature);
  builder_.add_minTemperature(minTemperature);
  builder_.add_conditionCode(conditionCode);
  return builder_.Finish();
}

struct Alerts FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef AlertsBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_VENT = 4,
    VT_PLUIEINONDATION = 6,
    VT_ORAGE = 8,
    VT_INONDATION = 10,
    VT_NEIGE = 12,
    VT_CANICULE = 14,
    VT_GRANDFROID = 16,
    VT_AVALANCHES = 18,
    VT_VAGUESUBMERSION = 20
  };
  uint8_t vent() const {
    return GetField<uint8_t>(VT_VENT, 0);
  }
  uint8_t pluieInondation() const {
    return GetField<uint8_t>(VT_PLUIEINONDATION, 0);
  }
  uint8_t orage() const {
    return GetField<uint8_t>(VT_ORAGE, 0);
  }
  uint8_t inondation() const {
    return GetField<uint8_t>(VT_INONDATION, 0);
  }
  uint8_t neige() const {
    return GetField<uint8_t>(VT_NEIGE, 0);
  }
  uint8_t canicule() const {
    return GetField<uint8_t>(VT_CANICULE, 0);
  }
  uint8_t grandFroid() const {
    return GetField<uint8_t>(VT_GRANDFROID, 0);
  }
  uint8_t avalanches() const {
    return GetField<uint8_t>(VT_AVALANCHES, 0);
  }
  uint8_t vagueSubmersion() const {
    return GetField<uint8_t>(VT_VAGUESUBMERSION, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_VENT, 1) &&
           VerifyField<uint8_t>(verifier, VT_PLUIEINONDATION, 1) &&
           VerifyField<uint8_t>(verifier, VT_ORAGE, 1) &&
           VerifyField<uint8_t>(verifier, VT_INONDATION, 1) &&
           VerifyField<uint8_t>(verifier, VT_NEIGE, 1) &&
           VerifyField<uint8_t>(verifier, VT_CANICULE, 1) &&
           VerifyField<uint8_t>(verifier, VT_GRANDFROID, 1) &&
           VerifyField<uint8_t>(verifier, VT_AVALANCHES, 1) &&
           VerifyField<uint8_t>(verifier, VT_VAGUESUBMERSION, 1) &&
           verifier.EndTable();
  }
};

struct AlertsBuilder {
  typedef Alerts Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_vent(uint8_t vent) {
    fbb_.AddElement<uint8_t>(Alerts::VT_VENT, vent, 0);
  }
  void add_pluieInondation(uint8_t pluieInondation) {
    fbb_.AddElement<uint8_t>(Alerts::VT_PLUIEINONDATION, pluieInondation, 0);
  }
  void add_orage(uint8_t orage) {
    fbb_.AddElement<uint8_t>(Alerts::VT_ORAGE, orage, 0);
  }
  void add_inondation(uint8_t inondation) {
    fbb_.AddElement<uint8_t>(Alerts::VT_INONDATION, inondation, 0);
  }
  void add_neige(uint8_t neige) {
    fbb_.AddElement<uint8_t>(Alerts::VT_NEIGE, neige, 0);
  }
  void add_canicule(uint8_t canicule) {
    fbb_.AddElement<uint8_t>(Alerts::VT_CANICULE, canicule, 0);
  }
  void add_grandFroid(uint8_t grandFroid) {
    fbb_.AddElement<uint8_t>(Alerts::VT_GRANDFROID, grandFroid, 0);
  }
  void add_avalanches(uint8_t avalanches) {
    fbb_.AddElement<uint8_t>(Alerts::VT_AVALANCHES, avalanches, 0);
  }
  void add_vagueSubmersion(uint8_t vagueSubmersion) {
    fbb_.AddElement<uint8_t>(Alerts::VT_VAGUESUBMERSION, vagueSubmersion, 0);
  }
  explicit AlertsBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Alerts> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Alerts>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Alerts> CreateAlerts(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t vent = 0,
    uint8_t pluieInondation = 0,
    uint8_t orage = 0,
    uint8_t inondation = 0,
    uint8_t neige = 0,
    uint8_t canicule = 0,
    uint8_t grandFroid = 0,
    uint8_t avalanches = 0,
    uint8_t vagueSubmersion = 0) {
  AlertsBuilder builder_(_fbb);
  builder_.add_vagueSubmersion(vagueSubmersion);
  builder_.add_avalanches(avalanches);
  builder_.add_grandFroid(grandFroid);
  builder_.add_canicule(canicule);
  builder_.add_neige(neige);
  builder_.add_inondation(inondation);
  builder_.add_orage(orage);
  builder_.add_pluieInondation(pluieInondation);
  builder_.add_vent(vent);
  return builder_.Finish();
}

inline const ohab_weather::Weather *GetWeather(const void *buf) {
  return ::flatbuffers::GetRoot<ohab_weather::Weather>(buf);
}

inline const ohab_weather::Weather *GetSizePrefixedWeather(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<ohab_weather::Weather>(buf);
}

inline bool VerifyWeatherBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<ohab_weather::Weather>(nullptr);
}

inline bool VerifySizePrefixedWeatherBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<ohab_weather::Weather>(nullptr);
}

inline void FinishWeatherBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<ohab_weather::Weather> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedWeatherBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<ohab_weather::Weather> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace ohab_weather

#endif  // FLATBUFFERS_GENERATED_OHABWEATHER_OHAB_WEATHER_H_
