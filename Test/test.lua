-- A simple test script
Platform.init();
Log.info("hi from lua!");

while not Input.isPressed(88) do
	Platform.update();
end

Platform.exit();