#ifndef GEMGA_SCREEN_RECORDER_HPP
#define GEMGA_SCREEN_RECORDER_HPP

#include "screen_recorder_interface.hpp"

namespace jt {
class ScreenRecorder : public ScreenRecorderInterface {
public:
    /// currently not supported by native build
    void toggleRecording() override;
    /// currently not supported by native build
    void startRecording() override;
    /// currently not supported by native build
    void stopRecording() override;
    /// currently not supported by native build
    void takeScreenshot() override;
    /// currently not supported by native build
    /// \return false
    bool isRecording() override;
};

} // namespace jt
#endif // GEMGA_SCREEN_RECORDER_HPP
