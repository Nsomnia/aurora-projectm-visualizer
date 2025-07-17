# VideoExporter Feature

The `VideoExporter` is responsible for capturing the application's visual output and encoding it into a video file using FFmpeg.

## Configuration

The behavior of the `VideoExporter` is configured in the `[recording]` section of the `config/default.toml` file.

### `ffmpeg_command`

This is the most critical setting. It is a template for the `ffmpeg` command that will be executed. The following placeholders are available:

*   `{WIDTH}`: The width of the output video.
*   `{HEIGHT}`: The height of the output video.
*   `{FPS}`: The frames per second of the output video.
*   `{AUDIO_FILE_PATH}`: The path to the audio file being visualized.
*   `{OUTPUT_PATH}`: The path where the final video will be saved.

**Important:** Do not add any shell quoting (like single or double quotes) around the placeholders in the `ffmpeg_command` string. The application will automatically handle the necessary escaping for file paths to prevent errors with special characters or spaces.

**Example:**

```toml
ffmpeg_command = 'ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -i - -i {AUDIO_FILE_PATH} -c:v libx265 -crf 32 -c:a copy -r {FPS} -shortest -preset slow -threads 0 -movflags +faststart {OUTPUT_PATH}'
```
