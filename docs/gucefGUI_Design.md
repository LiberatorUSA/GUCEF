# gucefGUI

Module `platform/gucefGUI`, namespace `GUCEF::GUI`, files `gucefGUI_*.h/.cpp`. One GUI interface for every backend: applications build forms of widgets, react to their events and give them images and fonts; a **driver plugin** (`plugins/GUI/guidriver...`) shows them with a toolkit of its own. Layouts are gucefGUI's own files, so the same form works with every driver. `platform/gucefGUI` holds no third party code.

Status: the library with all its widget types, layouts, themes, routed input, navigation and a software reference renderer; the drivers `guidriverNull` (headless) and `guidriverRmlUI` + `guidriverRmlUIOpenGL` (RmlUi, drawing exactly what the reference renderer draws, on OpenGL 3.3, OpenGL ES 3 and WebGL 2); the window drivers `guidriverXWinGL` (X11, OpenGL 3.3 core), `guidriverEGL` (offscreen), `guidriverAndroidGLES` (Android, OpenGL ES 3) and `guidriverEmscripten` (a canvas of a web page, WebGL 2) with the input driver `inputdriverEMSCRIPTEN`. Tests: `tests/gucefGUI_TestApp`, on Linux and in a browser.

## 1. Layers

```
 application ──► CForm / CFormEx ──► CWidget facades ──► CGUIContext ──► CWidgetHandler (per type, from the driver) ──► toolkit (RmlUi, ...)
                     │                                       │
                 CFormLayout (DataNode, DStore codecs)        ├──► images and bitmap fonts, theme, layout, routed events, focus, popup, modal roots
                                                             └──► CWindowContext (window driver plugin) ◄── input (gucefINPUT, CGUIInputAdapter)
```

* **Widgets are facades.** A `CWidget` holds the values of its properties, its state (hovered, pressed, focused, checked, disabled) and its children. It has no backend object of its own: the context passes changes to the driver's handler for the type once per frame (`FlushChanges()`), as property, layout, state, parent and child order calls.
* **Properties are described.** `CWidgetTypeRegistry` knows every widget type and its properties (`CPropertyDescriptor`: name, value type, default, flags, category, choices). Editors list them, layouts are checked against them, drivers map them. A value comes from the widget, else from the theme, else from the default; Visible, Enabled and Checked never come from the theme. Unknown names are kept and passed to the handler (backend hints).
* **The context does the behaviour.** `CGUIContext` lays out (UDim positions and sizes, alignment, the containers HBox, VBox, Grid and Margin with Padding, Spacing and Columns), hit tests, turns input into routed events, keeps the focus and the keyboard navigation, the popup and the modal roots, the theme and the named images and fonts. Drivers only show.
* **Drawing is described once.** `CWidgetPainter` turns a widget into 2D drawing commands (the commands of `CDrawSurface`): fills, borders, lines, points, images with a source rectangle and a tint, text in a bitmap font, clipping. `CSoftwareRenderer` executes them into a pixel map; drivers that draw widgets themselves execute the same commands, so a widget looks the same everywhere.

## 2. Widget types

| type | what it is |
|---|---|
| `Widget` | base: Position, Size, HAlign, VAlign, Visible, Enabled, Opacity, Clip, InputTransparent, Focusable, TabIndex, NavUp/Down/Left/Right, Tooltip, StyleClass |
| `Panel` (also `Form`, `HBox`, `VBox`, `Grid`, `Margin`) | a rectangle with BackgroundColour, BackgroundImage, BorderColour, BorderWidth; the containers place their children |
| `Window` | a panel with a title bar |
| `Label`, `Textbox` | text in a bitmap font, aligned, word wrapped |
| `Button`, `PushButton`, `Checkbox` | text or pictures per state (NormalImage, HoverImage, PushedImage, DisabledImage, CheckedImage) |
| `Editbox` | one line of text input: caret, selection, MaxLength, SelectAllOnFocus |
| `Listbox`, `Combobox`, `GridView`, `Treeview` | rows of items with selection, scrolling and hover |
| `Spinner`, `Slider`, `Scrollbar`, `ProgressBar` | values in a range |
| `TabControl`, `TabContentPane`, `MenuBar`, `PopupMenu` | pages and menus |
| `ImageFrame` | an image (registered name or VFS path), scale modes, animation frames |
| `DrawSurface` | a list of drawing commands the application builds and commits (custom views) |
| `VideoView`, `AudioPlayer` | gucefVIDEO and gucefAUDIO playback, shown as a dynamic image |
| `RenderContext` | a place for a renderer of the application |

## 3. Layout files

A layout is a `CDataNode` tree read and written with any DStore codec, chosen by the file extension (`.xml`, `.json`):

```xml
<GUILayout format="GUILayout" version="1">
    <Resources>
        <Image alias="logo" path="/data/logo.png" />
    </Resources>
    <Widget type="Panel" name="Window" position="0,0 0,0" size="1,0 1,0">
        <Property name="BackgroundImage" value="logo" />
        <Widget type="Button" name="Ok" position="0,10 0,20" size="0,80 0,24">
            <Property name="Text" value="OK" />
        </Widget>
        <BackendHints driver="RmlUi">
            <Property name="rcss:filter" value="blur(1px)" />
        </BackendHints>
    </Widget>
</GUILayout>
```

* Positions and sizes are UDim pairs `"scale,pixels scale,pixels"`; pixels may be negative.
* String lists (items of a list box, ...) are one value with the items separated by line breaks.
* `BackendHints` are read only by the driver they name. Unknown nodes and properties survive a load and a save.
* `CForm::LoadLayout()` builds the widgets (a file that is no gucefGUI layout goes to the driver's native loader, for example RML for RmlUi); `CForm::SaveLayout()` writes them back, only what differs from the defaults. `CFormLayout` validates, simplifies and migrates trees.

## 4. Input and events

* Input reaches a context in logical pixels through `InjectMouseMove()`, `InjectMouseButton()`, `InjectMouseWheel()`, `InjectKey()`, `InjectText()`, `InjectNavigation()`, or in window pixels through `ProcessWindowMouseMove()`. Tests, scripts and replays use the same calls as real input.
* `CGUIInputAdapter` feeds a context from gucefINPUT: the mice and keyboards of an input context (the window), buttons 0 left, 1 right, 2 middle, 3 and 4 the side buttons, the wheel in notches, keys with their modifiers, UTF-8 text.
* Widget events are routed: tunnel from the root to the parent of the target, the target, bubble back up, with `CRoutedEventData::handled` to stop them. Data: `CPointerEventData` (positions in context and widget coordinates, button, click count), `CWheelEventData`, `CKeyEventData`, `CTextEventData`, `CNavigationEventData`, `CFocusEventData`, `CValueChangedEventData`, `CItemEventData`. Keys nobody handles become navigation: Tab and the arrows move the focus (explicit neighbours, then spatially), Enter activates, Escape cancels.

## 5. Images, fonts, themes, presentation

* `RegisterImage( alias, pixels )`, `UpdateImage( alias )` for changed pixels (video frames), `ResolveImage()` (a registered alias, else a file in the VFS read with the gucefIMAGE codecs).
* `RegisterFont( name, CBitmapFont )`: a glyph atlas with per glyph source rectangle, offsets and advance, spacing, line height; drivers may add fonts of their own (`LoadFontFromAsset()`).
* `SetTheme()`: a `GUITheme` tree of `Style` nodes selecting on type, class, name and states.
* `SetLogicalResolution()` lays out at a fixed size; `SetPresentMode()` (nearest, linear, integer multiples, stretch) says how that picture fills the window; `SetCursor()` draws a picture at the pointer instead of the system cursor. `CaptureFrame()` gives the last frame at the logical resolution.
* `RenderFrame( deltaInMs )` updates animations and media, lays out, flushes changes to the driver and draws; a window context's redraw event calls it with the real time that passed. `FrameStartedEvent` lets the application draw its custom views just before.

## 6. Drivers

| plugin | registers | what it does |
|---|---|---|
| `guidriverNull` | GUI driver `Null` | keeps the widgets; `CaptureFrame()` draws with `CSoftwareRenderer`. Logic tests without a screen, the reference for the others |
| `guidriverRmlUI` | (library) | `CRmlUIDriver`, `CRmlUIContext`: an RmlUi document per root, an element per widget that draws the painter's commands (CPU clipped, geometry kept until the drawing changes), textures from the named images and font atlases, RML documents as native layouts, backend hints `rcss:<property>` and `rml:class` |
| `guidriverRmlUIOpenGL` | GUI driver `RmlUi` | RmlUi's OpenGL 3 renderer (GL 3.3, GLES 3, WebGL 2): the frame is drawn into a framebuffer of the logical size, presented scaled, captured with `glReadPixels` |
| `guidriverXWinGL` | window backend `XWinGL` | X11 window with an OpenGL 3.3 core context (legacy context as fallback), vsync as the video settings say, full screen, close requests, system cursor |
| `guidriverEGL` | window backend `EGL` | offscreen window: an EGL context (Mesa surfaceless when there is no display server) drawing into a framebuffer object |
| `guidriverAndroidGLES` | window backend `AndroidGLES` | the window of an Android app (the `android_app` window parameter) with an OpenGL ES 3 context, vsync as the video settings say |
| `guidriverEmscripten` | window backend `Emscripten` | the canvas of a web page (window parameter `CANVAS`, a CSS selector, `#canvas` by default) with a WebGL 2 context; full screen scaled with nearest filtering (the browser allows it only while it handles a key press or a click, a request made elsewhere waits for the next one); system cursor. Linked into the application: `guidriverEmscripten_GUCEFPlugin_Load()` registers it |
| `inputdriverEMSCRIPTEN` | input driver `EMSCRIPTEN` | the input of a web page for a canvas (context parameter `WINDOW`, the `WINDOW` property of the window): mouse in pixels of the drawing buffer, the wheel in notches, keys by their position (`code`) and the text they type; the browser keeps Ctrl / Meta shortcuts, F5 and F12; held keys are let go when the page loses the focus. Linked into the application: `inputdriverEMSCRIPTEN_GUCEFPlugin_Load()` |
| `guidriverMyGUI*`, `guidriverCEGUI*` | | older drivers that compile against the interface |

On the web there is no loading of modules: plugins are linked into the application and their entry points have the name of the plugin in front (`guidriverNull_GUCEFPlugin_Load()`, `guidriverRmlUIOpenGL_GUCEFPlugin_Load()`, ...), which the application calls itself.

Window contexts tell renderers about their GL context through properties: `GL_GETPROCADDRESS` (pointer of a function that loads GL entry points), `GL_FRAMEBUFFER` (the framebuffer that is the picture of the window, 0 for an on screen window), `GL_API` (`GL` or `GLES`), `GL_VERSION`; `WindowContextCloseRequestEvent` when the user closes the window.

## 7. How to write a driver

1. Derive from `CGUIDriver`: `CreateGUIContext()`, `GetWidgetHandler( type )`, `GetCapabilities()` (for example `CaptureFrame`, `BitmapFonts`, `DrawSurface`, `DynamicImages`, `NativeLayouts`, `BackendHints`), `GetInterfaceVersion()` returning `CGUIDriver::InterfaceVersion` (checked when the driver registers).
2. Derive from `CGUIContext` for the context: override `OnRender()` (draw the roots in `GetRootWidgets()` order, then `CWidgetPainter::PaintOverlay()`), `OnRootOrderChanged()`, `OnImageChanged()`, `CaptureFrame()`, `CreateFormBackend()` for native layouts. Call `DestroyAllWidgets()` in the destructor while the backend still exists.
3. Derive from `CWidgetHandler` per type (or one for all): `CreateNative()` / `DestroyNative()` (children are destroyed before their parent), `ApplyParent()`, `ApplyLayout()` (rectangles in context coordinates), `ApplyChildOrder()`, `ApplyState()`, `ApplyProperty()` (map names with `MapProperty()`; unknown names are hints), `RunCommand()`, `GetNativeHandle()`.
4. Draw widgets with `CWidgetPainter::Paint()` unless the toolkit has a better widget of its own; then the gucefGUI render tests (`RunRenderTests()` in `tests/gucefGUI_TestApp`) can run against the driver and compare with the reference.
5. Keep contexts weakly (`CTWeakPtr`): their owners decide when they go, before the window driver plugin unloads.

## 8. Tests

`tests/gucefGUI_TestApp`: properties, layout and containers, routed events, focus and navigation, every widget type, layout round trips through XML and JSON, media widgets, the input adapter, and the render tests run on `Null` and on `RmlUi` in an `EGL` window, including a scene of every kind of drawing that must have the same pixels on both, and that a context gives everything back to RmlUi before its renderer goes.

Built for the web (Emscripten, `.html` or `.js` with a page that has a canvas `#canvas`) the same tests run in a browser, with `RmlUi` drawing with WebGL 2 in the canvas, plus the browser tests: the window driver `Emscripten` and input from events of the page through `inputdriverEMSCRIPTEN` (mouse with the canvas shown at another size, wheel, keys, text, focus loss). Under node the tests that need a page skip, as do the layout tests that need the XML and JSON codec plugins, which a web build cannot load at run time.

## 9. Not supported yet

Win32GL on OpenGL 3.3 (it still makes a legacy context), a Wayland window and input driver (XWayland works), loading of the codec plugins in a web build (a static plugin registry), the layout editor, data binding, an LVGL driver.
