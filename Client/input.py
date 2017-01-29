from sdl2 import *

def apply_deadzone(x, deadzone, scale):
    if x < 0:
        return (scale * min(0,x+deadzone)) / (32768-deadzone)
    return (scale * max(0,x-deadzone)) / (32768-deadzone)

class Input():
    def __init__(self, deadzone, scale, trigscale):
        self.deadzone = deadzone
        self.scale = scale
        self.trigscale = trigscale
        # Initialize subsystem. Check if joystick exists, AND is a GameController.
        SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC)
        SDL_GameControllerAddMappingsFromFile(b"gamecontrollerdb.txt")
        self.haptic_available = False
        if SDL_NumJoysticks() > 0:
            if SDL_IsGameController(0) == 1:
                self.controller = SDL_GameControllerOpen(0)
                controllername = str(SDL_GameControllerName(self.controller))
                print("Initialized: " + controllername)
                # Initialize haptics if available.
                if SDL_NumHaptics() > 0:
                    self.haptics = SDL_HapticOpen(0)
                    if SDL_HapticRumbleSupported(self.haptics) > 0:
                        SDL_HapticRumbleInit(self.haptics)
                        self.haptic_available = True
        else:
            print("No game controller found.")

        self.inputs = {"lxaxis": 0, "lyaxis": 0, "rxaxis": 0, "ryaxis": 0,
                       "ltrigger": 0, "rtrigger": 0,
                       "up": 0, "down": 0, "left": 0, "right": 0,
                       "a": 0, "b": 0, "x": 0, "y": 0,
                       "l": 0, "r": 0, "start": 0, "back": 0}

    def rumble(self, intensity, length_ms):
        SDL_HapticRumblePlay(self.haptics, intensity, length_ms)

    def getState(self):
        SDL_JoystickUpdate();
        self.inputs["lxaxis"] = apply_deadzone( SDL_GameControllerGetAxis(self.controller, SDL_CONTROLLER_AXIS_LEFTX), self.deadzone, self.scale )
        self.inputs["lyaxis"] = apply_deadzone( SDL_GameControllerGetAxis(self.controller, SDL_CONTROLLER_AXIS_LEFTY), self.deadzone, self.scale )
        self.inputs["rxaxis"] = apply_deadzone( SDL_GameControllerGetAxis(self.controller, SDL_CONTROLLER_AXIS_RIGHTX), self.deadzone, self.scale )
        self.inputs["ryaxis"] = apply_deadzone( SDL_GameControllerGetAxis(self.controller, SDL_CONTROLLER_AXIS_RIGHTY), self.deadzone, self.scale )
        self.inputs["ltrigger"] = apply_deadzone( SDL_GameControllerGetAxis(self.controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT), 0, self.trigscale )
        self.inputs["rtrigger"] = apply_deadzone( SDL_GameControllerGetAxis(self.controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT), 0, self.trigscale )

        self.inputs["a"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_A)
        self.inputs["b"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_B)
        self.inputs["x"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_X)
        self.inputs["y"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_Y)
        self.inputs["l"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
        self.inputs["r"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
        self.inputs["back"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_BACK)
        self.inputs["start"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_START)
        self.inputs["up"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_DPAD_UP)
        self.inputs["down"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)
        self.inputs["left"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)
        self.inputs["right"] = SDL_GameControllerGetButton(self.controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
        return self.inputs
