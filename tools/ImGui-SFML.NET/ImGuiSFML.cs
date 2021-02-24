using System;
using System.Diagnostics;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using SFML.Graphics;
using SFML.System;
using SFML.Window;

namespace ImGuiNET
{
	public static class SFMLImpl
	{
		private static bool _windowHasFocus = false;
		private static readonly bool[] _mousePressed = new bool[] { false, false, false };
		private static readonly bool[] _touchDown = new bool[] { false, false, false };
		private static bool _mouseMoved = false;
		private static Vector2i _touchPos;
		private static Texture _fontTexture = null;

		private static readonly uint _nullJoystickId = Joystick.Count;
		public static uint JoystickId{ get; set; } = _nullJoystickId;

		private static readonly uint _nullJoystickButton = Joystick.ButtonCount;
		private static readonly uint[] _joystickMapping = new uint[(int)ImGuiNavInput.COUNT];

		private class StickInfo
		{
			public Joystick.Axis xAxis;
			public Joystick.Axis yAxis;

			public bool xInverted;
			public bool yInverted;

			public float threshold;
		}

		private static StickInfo _dPadInfo = new StickInfo();
		private static StickInfo _lStickInfo = new StickInfo();

		private static readonly Cursor[] _mouseCursors = new Cursor[(int)ImGuiMouseCursor.COUNT];
		private static readonly bool[] _mouseCursorLoaded = new bool[(int)ImGuiMouseCursor.COUNT];

		private static string ClipboadText
		{
			get => Clipboard.Contents;
			set => Clipboard.Contents = value;
		}


		public static void Init(RenderWindow window, bool loadDefaultFont = true)
		{
			Init(window, window, loadDefaultFont);
		}

		public static void Init(Window window, RenderTarget target, bool loadDefaultFont = true)
		{
			Init(window, new Vector2f(target.Size.X, target.Size.Y), loadDefaultFont);
		}

		public static void Init(Window window, Vector2f displaySize, bool loadDefaultFont = true)
		{
			ImGui.CreateContext();
			var io = ImGui.GetIO();

			// tell ImGui which features we support
			io.BackendFlags |= ImGuiBackendFlags.HasGamepad;
			io.BackendFlags |= ImGuiBackendFlags.HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags.HasSetMousePos;

			// init keyboard mapping
			io.KeyMap[(int)ImGuiKey.Tab] = (int)Keyboard.Key.Tab;
			io.KeyMap[(int)ImGuiKey.LeftArrow] = (int)Keyboard.Key.Left;
			io.KeyMap[(int)ImGuiKey.RightArrow] = (int)Keyboard.Key.Right;
			io.KeyMap[(int)ImGuiKey.UpArrow] = (int)Keyboard.Key.Up;
			io.KeyMap[(int)ImGuiKey.DownArrow] = (int)Keyboard.Key.Down;
			io.KeyMap[(int)ImGuiKey.PageUp] = (int)Keyboard.Key.PageUp;
			io.KeyMap[(int)ImGuiKey.PageDown] = (int)Keyboard.Key.PageDown;
			io.KeyMap[(int)ImGuiKey.Home] = (int)Keyboard.Key.Home;
			io.KeyMap[(int)ImGuiKey.End] = (int)Keyboard.Key.End;
			io.KeyMap[(int)ImGuiKey.Insert] = (int)Keyboard.Key.Insert;
			io.KeyMap[(int)ImGuiKey.Delete] = (int)Keyboard.Key.Delete;
			io.KeyMap[(int)ImGuiKey.Backspace] = (int)Keyboard.Key.Backspace;
			io.KeyMap[(int)ImGuiKey.Space] = (int)Keyboard.Key.Space;
			io.KeyMap[(int)ImGuiKey.Enter] = (int)Keyboard.Key.Enter;
			io.KeyMap[(int)ImGuiKey.Escape] = (int)Keyboard.Key.Escape;
			io.KeyMap[(int)ImGuiKey.A] = (int)Keyboard.Key.A;
			io.KeyMap[(int)ImGuiKey.C] = (int)Keyboard.Key.C;
			io.KeyMap[(int)ImGuiKey.V] = (int)Keyboard.Key.V;
			io.KeyMap[(int)ImGuiKey.X] = (int)Keyboard.Key.X;
			io.KeyMap[(int)ImGuiKey.Y] = (int)Keyboard.Key.Y;
			io.KeyMap[(int)ImGuiKey.Z] = (int)Keyboard.Key.Z;

			JoystickId = GetConnectedJoystickId();

			for (var i = 0u; i < (uint)ImGuiNavInput.COUNT; ++i)
			{
				_joystickMapping[i] = _nullJoystickButton;
			}

			InitDefaultJoystickMapping();

			// init rendering
			io.DisplaySize = new Vector2(displaySize.X, displaySize.Y);

			// TODO: Implement
			// clipboard
			// io.SetClipboardTextFn = ClipboardText;
			// io.GetClipboardTextFn = getClipboadText;

			// load mouse cursors
			for (var i = 0; i < (int)ImGuiMouseCursor.COUNT; ++i)
			{
				_mouseCursorLoaded[i] = false;
			}

			LoadMouseCursor(ImGuiMouseCursor.Arrow, Cursor.CursorType.Arrow);
			LoadMouseCursor(ImGuiMouseCursor.TextInput, Cursor.CursorType.Text);
			LoadMouseCursor(ImGuiMouseCursor.ResizeAll, Cursor.CursorType.SizeAll);
			LoadMouseCursor(ImGuiMouseCursor.ResizeNS, Cursor.CursorType.SizeVertical);
			LoadMouseCursor(ImGuiMouseCursor.ResizeEW, Cursor.CursorType.SizeHorinzontal);
			LoadMouseCursor(ImGuiMouseCursor.ResizeNESW, Cursor.CursorType.SizeBottomLeftTopRight);
			LoadMouseCursor(ImGuiMouseCursor.ResizeNWSE, Cursor.CursorType.SizeTopLeftBottomRight);
			LoadMouseCursor(ImGuiMouseCursor.Hand, Cursor.CursorType.Hand);

			if (loadDefaultFont)
			{
				// this will load default font automatically
				// No need to call AddDefaultFont
				UpdateFontTexture();
			}

			_windowHasFocus = window.HasFocus();

			window.MouseMoved += OnMouseMoved;
			window.MouseButtonPressed += OnMouseButtonPressed;
			window.MouseButtonReleased += OnMouseButtonReleased;
			window.TouchBegan += OnTouchBegan;
			window.TouchEnded += OnTouchEnded;
			window.MouseWheelScrolled += OnMouseWheelScrolled;
			window.KeyPressed += OnKeyPressed;
			window.KeyReleased += OnKeyReleased;
			window.TextEntered += OnTextEntered;
			window.JoystickConnected += OnJoystickConnected;
			window.JoystickDisconnected += OnJoystickDisconnected;
			window.GainedFocus += OnGainedFocus;
			window.LostFocus += OnLostFocus;
		}


		private static void OnMouseMoved(object sender, MouseMoveEventArgs args)
		{
			_mouseMoved = true;
		}

		private static void OnMouseButtonPressed(object sender, MouseButtonEventArgs args)
		{
			var button = (int)args.Button;
			if (button >= 0 && button < 3)
			{
				_mousePressed[(int)args.Button] = true;
			}
		}

		private static void OnMouseButtonReleased(object sender, MouseButtonEventArgs args)
		{
			var button = (int)args.Button;
			if (button >= 0 && button < 3)
			{
				_mousePressed[(int)args.Button] = false;
			}
		}

		private static void OnTouchBegan(object sender, TouchEventArgs args)
		{
			_mouseMoved = false;
			var button = args.Finger;
			if (button < 3)
			{
				_touchDown[args.Finger] = true;
			}
		}

		private static void OnTouchEnded(object sender, TouchEventArgs args)
		{
			var button = args.Finger;
			if (button < 3)
			{
				_touchDown[args.Finger] = false;
			}
		}

		private static void OnMouseWheelScrolled(object sender, MouseWheelScrollEventArgs args)
		{
			var io = ImGui.GetIO();
			switch (args.Wheel)
			{
				case Mouse.Wheel.VerticalWheel:
				case Mouse.Wheel.HorizontalWheel when io.KeyShift:
					io.MouseWheel += args.Delta;
					break;
				case Mouse.Wheel.HorizontalWheel:
					io.MouseWheelH += args.Delta;
					break;
				default:
					throw new ArgumentOutOfRangeException();
			}
		}

		private static void OnKeyPressed(object sender, KeyEventArgs args)
		{
			var io = ImGui.GetIO();
			io.KeysDown[(int)args.Code] = true;
		}

		private static void OnKeyReleased(object sender, KeyEventArgs args)
		{
			var io = ImGui.GetIO();
			io.KeysDown[(int)args.Code] = false;
		}

		private static void OnTextEntered(object sender, TextEventArgs args)
		{
			var io = ImGui.GetIO();
			// Don't handle the event for unprintable characters
			if (args.Unicode[0] < ' ' || args.Unicode[0] == 127)
			{
				return;
			}

			io.AddInputCharacter(args.Unicode[0]);
		}

		private static void OnJoystickConnected(object sender, JoystickConnectEventArgs args)
		{
			if (JoystickId == _nullJoystickId)
			{
				JoystickId = args.JoystickId;
			}
		}

		private static void OnJoystickDisconnected(object sender, JoystickConnectEventArgs args)
		{
			if (JoystickId == args.JoystickId)
			{
				JoystickId = GetConnectedJoystickId();
			}
		}

		private static void OnGainedFocus(object sender, EventArgs args)
		{
			_windowHasFocus = true;
		}

		private static void OnLostFocus(object sender, EventArgs args)
		{
			_windowHasFocus = false;
		}


		public static void Update(RenderWindow window, Time dt)
		{
			Update(window, window, dt);
		}

		public static void Update(Window window, RenderTarget target, Time dt)
		{
			// Update OS/hardware mouse cursor if imgui isn't drawing a software cursor
			UpdateMouseCursor(window);

			if (!_mouseMoved)
			{
				if (Touch.IsDown(0)) _touchPos = Touch.GetPosition(0, window);

				Update(_touchPos, (Vector2f)target.Size, dt);
			}
			else
			{
				Update(Mouse.GetPosition(window), (Vector2f)target.Size, dt);
			}

			if (ImGui.GetIO().MouseDrawCursor)
			{
				// Hide OS mouse cursor if imgui is drawing it
				window.SetMouseCursorVisible(false);
			}
		}

		public static void Update(Vector2i mousePos, Vector2f displaySize, Time dt)
		{
			var io = ImGui.GetIO();
			io.DisplaySize = new Vector2(displaySize.X, displaySize.Y);

			io.DeltaTime = dt.AsSeconds();

			if (_windowHasFocus)
			{
				if (io.WantSetMousePos)
				{
					var newMousePos = new Vector2i((int)io.MousePos.X, (int)io.MousePos.Y);
					Mouse.SetPosition(newMousePos);
				}
				else
				{
					io.MousePos = new Vector2(mousePos.X, mousePos.Y);
				}

				for (var i = 0; i < 3; i++)
				{
					io.MouseDown[i] = _touchDown[i] || Touch.IsDown((uint)i) || _mousePressed[i] ||
					                  Mouse.IsButtonPressed((Mouse.Button)i);

					_mousePressed[i] = false;
					_touchDown[i] = false;
				}
			}

			// Update Ctrl, Shift, Alt, Super state
			io.KeyCtrl = io.KeysDown[(int)Keyboard.Key.LControl] || io.KeysDown[(int)Keyboard.Key.RControl];
			io.KeyAlt = io.KeysDown[(int)Keyboard.Key.LAlt] || io.KeysDown[(int)Keyboard.Key.RAlt];
			io.KeyShift = io.KeysDown[(int)Keyboard.Key.LShift] || io.KeysDown[(int)Keyboard.Key.RShift];
			io.KeySuper = io.KeysDown[(int)Keyboard.Key.LSystem] || io.KeysDown[(int)Keyboard.Key.RSystem];

			Debug.Assert(io.Fonts.Fonts.Size > 0); // You forgot to create and set up font
			// atlas (see createFontTexture)

			//gamepad navigation
			Joystick.Update(); // joystick detection issue fix in SFML.NET
			if ((io.ConfigFlags & ImGuiConfigFlags.NavEnableGamepad) != 0 && JoystickId != _nullJoystickId)
			{
				UpdateJoystickActionState(io, ImGuiNavInput.Activate);
				UpdateJoystickActionState(io, ImGuiNavInput.Cancel);
				UpdateJoystickActionState(io, ImGuiNavInput.Input);
				UpdateJoystickActionState(io, ImGuiNavInput.Menu);

				UpdateJoystickActionState(io, ImGuiNavInput.FocusPrev);
				UpdateJoystickActionState(io, ImGuiNavInput.FocusNext);

				UpdateJoystickActionState(io, ImGuiNavInput.TweakSlow);
				UpdateJoystickActionState(io, ImGuiNavInput.TweakFast);

				UpdateJoystickDPadState(io);
				UpdateJoystickLStickState(io);
			}

			ImGui.NewFrame();
		}


		public static void Render(RenderTarget target)
		{
			target.ResetGLStates();
			ImGui.Render();
			RenderDrawLists(ImGui.GetDrawData(), ImGui.GetIO());
		}

		public static void Render()
		{
			ImGui.Render();
			RenderDrawLists(ImGui.GetDrawData(), ImGui.GetIO());
		}


		public static void Shutdown()
		{
			ImGui.GetIO().Fonts.TexID = IntPtr.Zero;
			_fontTexture.Dispose();
			for (var i = 0; i < (int)ImGuiMouseCursor.COUNT; ++i)
			{
				if (_mouseCursors[i] != null)
				{
					_mouseCursors[i].Dispose();
				}
			}

			ImGui.DestroyContext();
		}


		public static void UpdateFontTexture()
		{
			var io = ImGui.GetIO();
			io.Fonts.GetTexDataAsRGBA32(out IntPtr pixels, out var width, out var height, out var bpp);

			_fontTexture = new Texture((uint)width, (uint)height);

			var size = width * height * bpp;
			var byteArray = new byte[size];
			Marshal.Copy(pixels, byteArray, 0, size);

			_fontTexture.Update(byteArray);

			io.Fonts.SetTexID(ConvertGlTextureHandleToImTextureId(_fontTexture.NativeHandle));
		}

		public static Texture GetFontTexture()
		{
			return _fontTexture;
		}

		public static void SetJoystickMapping(int action, uint joystickButton)
		{
			Debug.Assert(action < (int)ImGuiNavInput.COUNT);
			Debug.Assert(joystickButton < (int)Joystick.ButtonCount);
			_joystickMapping[action] = joystickButton;
		}

		public static void SetDPadXAxis(Joystick.Axis dPadXAxis, bool inverted = false)
		{
			_dPadInfo.xAxis = dPadXAxis;
			_dPadInfo.xInverted = inverted;
		}

		public static void SetDPadYAxis(Joystick.Axis dPadYAxis, bool inverted = false)
		{
			_dPadInfo.yAxis = dPadYAxis;
			_dPadInfo.yInverted = inverted;
		}

		public static void SetLStickXAxis(Joystick.Axis lStickXAxis, bool inverted = false)
		{
			_lStickInfo.xAxis = lStickXAxis;
			_lStickInfo.xInverted = inverted;
		}

		public static void SetLStickYAxis(Joystick.Axis lStickYAxis, bool inverted = false)
		{
			_lStickInfo.xAxis = lStickYAxis;
			_lStickInfo.xInverted = inverted;
		}

		// Image overloads
		public static void Image(Texture texture, Color tintColor, Color borderColor)
		{
			Image(texture, (Vector2f)texture.Size, tintColor, borderColor);
		}

		public static void Image(Texture texture, Vector2f size, Color tintColor, Color borderColor)
		{
			var textureId = ConvertGlTextureHandleToImTextureId(texture.NativeHandle);

			ImGui.Image(textureId, new Vector2(size.X, size.Y), new Vector2(0, 0), new Vector2(1, 1),
				ToImColor(tintColor).Value, ToImColor(borderColor).Value);
		}

		public static void Image(RenderTexture texture, Color tintColor, Color borderColor)
		{
			Image(texture, (Vector2f)texture.Size, tintColor, borderColor);
		}

		public static void Image(RenderTexture texture, Vector2f size, Color tintColor,
			Color borderColor)
		{
			var textureId = ConvertGlTextureHandleToImTextureId(texture.Texture.NativeHandle);

			ImGui.Image(textureId, new Vector2(size.X, size.Y), new Vector2(0, 1), new Vector2(1, 0),
				ToImColor(tintColor).Value, ToImColor(borderColor).Value);
		}

		public static void Image(Sprite sprite, Color tintColor, Color borderColor)
		{
			var bounds = sprite.GetGlobalBounds();
			Image(sprite, new Vector2f(bounds.Width, bounds.Height), tintColor, borderColor);
		}

		public static void Image(Sprite sprite, Vector2f size, Color tintColor, Color borderColor)
		{
			var texture = sprite.Texture;
			// sprite without texture cannot be drawn
			if (texture is null)
			{
				return;
			}

			var textureSize = (Vector2f)texture.Size;
			var textureRect = sprite.TextureRect;

			var uv0 = new Vector2(textureRect.Left / textureSize.X, textureRect.Top / textureSize.Y);
			var uv1 = new Vector2((textureRect.Left + textureRect.Width) / textureSize.X,
				(textureRect.Top + textureRect.Height) / textureSize.Y);

			var textureId = ConvertGlTextureHandleToImTextureId(texture.NativeHandle);
			ImGui.Image(textureId, new Vector2(size.X, size.Y), uv0, uv1,
				ToImColor(tintColor).Value, ToImColor(borderColor).Value);
		}


		// ImageButton overloads
		public static bool ImageButton(Texture texture, int framePadding, Color bgColor, Color tintColor)
		{
			return ImageButton(texture, new Vector2f(texture.Size.X, texture.Size.Y), framePadding, bgColor,
				tintColor);
		}

		public static bool ImageButton(Texture texture, Vector2f size, int framePadding, Color bgColor,
			Color tintColor)
		{
			var textureSize = texture.Size;
			return ImageButtonImpl(texture, new FloatRect(0, 0, textureSize.X, textureSize.Y),
				size, framePadding, bgColor, tintColor);
		}

		public static bool ImageButton(Sprite sprite, int framePadding, Color bgColor, Color tintColor)
		{
			var spriteSize = sprite.GetGlobalBounds();
			return ImageButton(sprite, new Vector2f(spriteSize.Width, spriteSize.Height),
				framePadding, bgColor, tintColor);
		}

		public static bool ImageButton(Sprite sprite, Vector2f size, int framePadding, Color bgColor,
			Color tintColor)
		{
			var texture = sprite.Texture;
			if (texture == null)
			{
				return false;
			}

			var tr = sprite.TextureRect;

			return ImageButtonImpl(texture, new FloatRect(tr.Left, tr.Top, tr.Width, tr.Height), size,
				framePadding, bgColor, tintColor);
		}


		// Draw_list overloads. All positions are in relative coordinates (relative to top-left of the current window)
		public static void DrawLine(Vector2f a, Vector2f b, Color col, float thickness = 1.0f)
		{
			var drawList = ImGui.GetWindowDrawList();
			var pos = ImGui.GetCursorScreenPos();
			drawList.AddLine(new Vector2(a.X + pos.X, a.Y + pos.Y), new Vector2(b.X + pos.X, b.Y + pos.Y),
				ColorConvertFloat4ToU32(ToImColor(col).Value), thickness);
		}

		public static void DrawRect(FloatRect rect, Color color, float rounding = 0.0f, int roundingCorners =
			0x0F, float thickness = 1.0f)
		{
			var drawList = ImGui.GetWindowDrawList();
			drawList.AddRect(GetTopLeftAbsolute(rect), GetDownRightAbsolute(rect),
				ColorConvertFloat4ToU32(ToImColor(color).Value), rounding, (ImDrawCornerFlags)roundingCorners,
				thickness);
		}

		public static void DrawRectFilled(FloatRect rect, Color color, float rounding = 0.0f,
			int roundingCorners = 0x0F)
		{
			var drawList = ImGui.GetWindowDrawList();
			drawList.AddRectFilled(GetTopLeftAbsolute(rect), GetDownRightAbsolute(rect),
				ColorConvertFloat4ToU32(ToImColor(color).Value), rounding, (ImDrawCornerFlags)roundingCorners);
		}

		// various helper functions

		private static ImColor ToImColor(Color c)
		{
			return new ImColor { Value = new Vector4(c.R, c.G, c.B, c.A) };
		}

		private static Vector2 GetTopLeftAbsolute(FloatRect rect)
		{
			var pos = ImGui.GetCursorScreenPos();
			return new Vector2(rect.Left + pos.X, rect.Top + pos.Y);
		}

		private static Vector2 GetDownRightAbsolute(FloatRect rect)
		{
			var pos = ImGui.GetCursorScreenPos();
			return new Vector2(rect.Left + rect.Width + pos.X, rect.Top + rect.Height + pos.Y);
		}


		private static unsafe IntPtr ConvertGlTextureHandleToImTextureId(uint glTextureHandle)
		{
			var textureId = 0;
			long size = Unsafe.SizeOf<uint>();
			Buffer.MemoryCopy(&glTextureHandle, &textureId, size, size);
			return new IntPtr(textureId);
		}

		private static unsafe uint ConvertImTextureIdToGlTextureHandle(IntPtr imTexId)
		{
			uint glTextureHandle = 0;
			long size = Unsafe.SizeOf<uint>();
			Buffer.MemoryCopy(imTexId.ToPointer(), new IntPtr(&glTextureHandle).ToPointer(), size, size);
			return glTextureHandle;
		}

		// Rendering callback
		[DllImport("RenderDataImpl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		private static extern void RenderDrawLists(ImDrawDataPtr drawData, ImGuiIOPtr io);

		// return first id of connected joystick
		private static uint GetConnectedJoystickId()
		{
			Joystick.Update(); // joystick detection issue fix in SFML.NET

			for (var i = 0u; i < Joystick.Count; ++i)
			{
				if (Joystick.IsConnected(i)) return i;
			}

			return _nullJoystickId;
		}

		private static void InitDefaultJoystickMapping()
		{
			SetJoystickMapping((int)ImGuiNavInput.Activate, 0);
			SetJoystickMapping((int)ImGuiNavInput.Cancel, 1);
			SetJoystickMapping((int)ImGuiNavInput.Input, 3);
			SetJoystickMapping((int)ImGuiNavInput.Menu, 2);
			SetJoystickMapping((int)ImGuiNavInput.FocusPrev, 4);
			SetJoystickMapping((int)ImGuiNavInput.FocusNext, 5);
			SetJoystickMapping((int)ImGuiNavInput.TweakSlow, 4);
			SetJoystickMapping((int)ImGuiNavInput.TweakFast, 5);

			SetDPadXAxis(Joystick.Axis.PovX);
			SetDPadXAxis(Joystick.Axis.PovX);

			if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
			{
				SetDPadYAxis(Joystick.Axis.PovY, true);
			}
			else
			{
				SetDPadYAxis(Joystick.Axis.PovY);
			}

			SetLStickXAxis(Joystick.Axis.X);
			SetLStickYAxis(Joystick.Axis.Y);

			_dPadInfo.threshold = 5f;
			_lStickInfo.threshold = 5f;
		}

		private static void UpdateJoystickActionState(ImGuiIOPtr io, ImGuiNavInput action)
		{
			var isPressed = Joystick.IsButtonPressed(JoystickId, _joystickMapping[(int)action]);
			io.NavInputs[(int)action] = isPressed ? 1f : 0f;
		}

		private static void UpdateJoystickDPadState(ImGuiIOPtr io)
		{
			var dpadXPos = Joystick.GetAxisPosition(JoystickId, _dPadInfo.xAxis);
			if (_dPadInfo.xInverted) dpadXPos = -dpadXPos;

			var dpadYPos = Joystick.GetAxisPosition(JoystickId, _dPadInfo.yAxis);
			if (_dPadInfo.yInverted) dpadYPos = -dpadYPos;

			io.NavInputs[(int)ImGuiNavInput.DpadLeft] = dpadXPos < -_dPadInfo.threshold ? 1f : 0f;
			io.NavInputs[(int)ImGuiNavInput.DpadRight] = dpadXPos > _dPadInfo.threshold ? 1f : 0f;

			io.NavInputs[(int)ImGuiNavInput.DpadUp] = dpadYPos < -_dPadInfo.threshold ? 1f : 0f;
			io.NavInputs[(int)ImGuiNavInput.DpadDown] = dpadYPos > _dPadInfo.threshold ? 1f : 0f;
		}

		private static void UpdateJoystickLStickState(ImGuiIOPtr io)
		{
			var lStickXPos = Joystick.GetAxisPosition(JoystickId, _lStickInfo.xAxis);
			if (_lStickInfo.xInverted) lStickXPos = -lStickXPos;

			var lStickYPos = Joystick.GetAxisPosition(JoystickId, _lStickInfo.yAxis);
			if (_lStickInfo.yInverted) lStickYPos = -lStickYPos;

			if (lStickXPos < -_lStickInfo.threshold)
			{
				io.NavInputs[(int)ImGuiNavInput.LStickLeft] = MathF.Abs(lStickXPos / 100f);
			}

			if (lStickXPos > _lStickInfo.threshold)
			{
				io.NavInputs[(int)ImGuiNavInput.LStickRight] = lStickXPos / 100f;
			}

			if (lStickYPos < -_lStickInfo.threshold)
			{
				io.NavInputs[(int)ImGuiNavInput.LStickUp] = MathF.Abs(lStickYPos / 100f);
			}

			if (lStickYPos > _lStickInfo.threshold)
			{
				io.NavInputs[(int)ImGuiNavInput.LStickDown] = lStickYPos / 100f;
			}
		}

		// mouse cursors

		private static void LoadMouseCursor(ImGuiMouseCursor imguiCursorType, Cursor.CursorType sfmlCursorType)
		{
			_mouseCursors[(int)imguiCursorType] = new Cursor(sfmlCursorType);
			_mouseCursorLoaded[(int)imguiCursorType] = true;
		}

		private static void UpdateMouseCursor(Window window)
		{
			var io = ImGui.GetIO();
			if ((io.ConfigFlags & ImGuiConfigFlags.NoMouseCursorChange) != 0) return;

			var cursor = ImGui.GetMouseCursor();
			if (io.MouseDrawCursor || cursor == ImGuiMouseCursor.None)
			{
				window.SetMouseCursorVisible(false);
			}
			else
			{
				window.SetMouseCursorVisible(true);

				var sfmlCursor = _mouseCursors[(int)cursor] ?? _mouseCursors[(int)ImGuiMouseCursor.Arrow];
				window.SetMouseCursor(sfmlCursor);
			}
		}

		private static uint ColorConvertFloat4ToU32(Vector4 c)
		{
			var src = new[] { (byte)c.X, (byte)c.Y, (byte)c.Z, (byte)c.W };
			return BitConverter.ToUInt32(src);
		}

		private static bool ImageButtonImpl(Texture texture, FloatRect textureRect, Vector2f size, int framePadding,
			Color bgColor, Color tintColor)
		{
			var textureSize = texture.Size;

			var uv0 = new Vector2(textureRect.Left / textureSize.X, textureRect.Top / textureSize.Y);
			var uv1 = new Vector2((textureRect.Left + textureRect.Width) / textureSize.X,
				(textureRect.Top + textureRect.Height) / textureSize.Y);

			var textureId = ConvertGlTextureHandleToImTextureId(texture.NativeHandle);
			return ImGui.ImageButton(textureId, new Vector2(size.X, size.Y), uv0, uv1, framePadding,
				ToImColor(bgColor).Value, ToImColor(tintColor).Value);
		}
	}
}