-- A simple test script
Platform.init();
Log.info("hi from lua!");

Renderer.setDefault2D();
Renderer.setClearColor(255, 0, 0, 255);

while not Input.isPressed(88) do
    Renderer.beginFrame();
	Renderer.clear();
	
	Renderer.endFrame();
	Platform.update();
end

Platform.exit();