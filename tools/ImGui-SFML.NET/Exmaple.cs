using System;
using System.Numerics;
using System.Text;
using ImGuiNET;
using SFML.Graphics;
using SFML.System;
using SFML.Window;

namespace ImGuiSFML_Example
{
	class Program
	{
		static void Main(string[] args)
		{
			var window = new RenderWindow(new VideoMode(640, 480), "");
			window.SetFramerateLimit(60);
			ImGuiSFML.Init(window);

			var io = ImGui.GetIO();
			io.ConfigFlags |= ImGuiConfigFlags.NavEnableGamepad;
			io.ConfigFlags |= ImGuiConfigFlags.NavEnableKeyboard;

			var bgColor = new Color();
			var color = new Vector3(0f, 0f, 0f);
			var windowTitle = Encoding.ASCII.GetBytes("ImGui + SFML = <3");

			window.Closed += (sender, e) => window.Close();

			window.SetTitle(Encoding.ASCII.GetString(windowTitle));
			var deltaClock = new Clock();
			while (window.IsOpen)
			{
				Joystick.Update();
				window.DispatchEvents();
				ImGuiSFML.Update(window, deltaClock.Restart());

				ImGui.Begin("Hello World!");
				
				if (ImGui.ColorEdit3("Background color", ref color)) 
				{
					bgColor.R = (byte)(color.X * 255f);
					bgColor.G = (byte)(color.Y * 255f);
					bgColor.B = (byte)(color.Z * 255f);
				}
				
				ImGui.InputText("Window title", windowTitle, 500);
				
				if (ImGui.Button("Update window title")) 
				{
					window.SetTitle(Encoding.ASCII.GetString(windowTitle));
				}
				
				ImGui.End();

				window.Clear(bgColor);
				ImGuiSFML.Render();
				window.Display();
			}

			ImGuiSFML.Shutdown();
		}
	}
}