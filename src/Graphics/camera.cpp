#include "camera.h"
#include "../Program/InputCache.h"
#include "../Program/Program.h"

// Konstruktori
Camera::Camera(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget, const glm::vec3& upVector)
{

    defaultPosition_ = cameraPosition;
    position_ = cameraPosition;
    target_ = cameraTarget;
    up_ = upVector;

    //rotateCamera(120,10);
    rotateCamera(0,0);
    auto ic = InputCache::getInstance();
//    ic->register_lambda_function(EventType::KEYBOARD_MOUSE,
//              [&](const InputCache* c) {  
//              auto deltaTime2 = c->get_timeDelta();
//              Log::getWarning().log("Deleta on %",deltaTime2);
//              double deltaTime3 = 1.0 + deltaTime2 / 1000000.0;
//              Log::getWarning().log("Deleta double on %",deltaTime3);
//              this->deltaTime = float(deltaTime3);
//              /* Pyydetään SDL:n näppäintila */
//              //const Uint8* keystate = ic->get_keyboardState() ;
//
//              /* Shift hidastaa liikkumisnopeutta */
//              float speedMultiplier = this->camSpeed;
//              if (c->isKeyDown(SDL_SCANCODE_LSHIFT) || c->isKeyDown(SDL_SCANCODE_RSHIFT) )
//              {
//                  speedMultiplier *= 0.3f;
//              }
//
//              /* WASD-näppäimet */
//              if(c->isKeyDown(SDL_SCANCODE_UP) || c->isKeyDown(SDL_SCANCODE_W)) {
//                  Log::getWarning().log("uppppppppppppp");
//                  this->position_ += this->front_ * speedMultiplier * this->deltaTime;
//              }
//
//              if(c->isKeyDown(SDL_SCANCODE_DOWN) || c->isKeyDown(SDL_SCANCODE_S))
//                  this->position_ -= this->front_ * speedMultiplier * this->deltaTime;
//
//              if(c->isKeyDown(SDL_SCANCODE_LEFT) || c->isKeyDown(SDL_SCANCODE_A))
//                  this->position_ -= glm::normalize(glm::cross(this->front_, this->up_)) * speedMultiplier * this->deltaTime;
//
//              if(c->isKeyDown(SDL_SCANCODE_RIGHT) || c->isKeyDown(SDL_SCANCODE_D))
//                  this->position_ += glm::normalize(glm::cross(this->front_, this->up_)) * speedMultiplier * this->deltaTime;
//
//              /* Ylös ja alas liikkuminen */
//              if(c->isKeyDown(SDL_SCANCODE_V))
//                  this->position_ += glm::normalize(this->up_) * speedMultiplier * this->deltaTime;
//
//              if(c->isKeyDown(SDL_SCANCODE_C))
//                  this->position_ -= glm::normalize(this->up_) * speedMultiplier * this->deltaTime;
//              Log::getWarning().log("position: %",this->position_);
//              this->update(this->deltaTime); // TODO: this->deltaTime does nothing
//              });

    ic->register_lambda_function(EventType::KEYBOARD_MOUSE,
              [&](const InputCache* c) {  
                if (Program::rayCamera == false && pRaycamera) return;
                if (Program::rayCamera == true && !pRaycamera) return;
                if (c->isMousePressed(SDL_BUTTON_LEFT))
                {
                  auto pos = c->getCurrent_mousePosition();
                  this->lastMouseX = pos.x;
                  this->lastMouseY = pos.y;
                }

                if (c->isMouseMoving() && c->isMouseDown(SDL_BUTTON_LEFT))
                {
                  auto pos = c->getCurrent_mousePosition();
                  this->rotateCamera((pos.x - this->lastMouseX) * this->camSensitivity * this->deltaTime,
                                    (this->lastMouseY - pos.y) * this->camSensitivity * this->deltaTime);
                  this->lastMouseX = pos.x;
                  this->lastMouseY = pos.y;
                }
                this->update(0.0f);
              });


//    case (SDL_MOUSEBUTTONDOWN): /* Klikkaus. Sijainti talteen sulavampaa liikettä varten */
//        lastMouseX = inputEvent.motion.x;
//        lastMouseY = inputEvent.motion.y;
//    case (SDL_MOUSEMOTION):
//        switch (inputEvent.motion.state)
//        {
//        case (SDL_BUTTON_LMASK): /* Hiiren vasen nappi pohjassa */
//            rotateCamera((inputEvent.motion.x - lastMouseX) * camSensitivity * deltaTime,
//                         (lastMouseY - inputEvent.motion.y) * camSensitivity * deltaTime);
//            /* Hiiren sijainti talteen muutoksen laskemista varten */
//            lastMouseX = inputEvent.motion.x;
//            lastMouseY = inputEvent.motion.y;
//            break;
//    }
}

// Destruktori
Camera::~Camera()
{
}


/**
 * Palauttaa kameran matriisin
 * @return Matrix
 */
glm::mat4 Camera::getMatrix() const
{
    return view;
}

/**
 * Palauttaa kameran sijainnin
 * @return position
 */
glm::vec3 Camera::getPosition() const
{
    return position_;
}

glm::vec3 Camera::getTarget() const
{
    return target_;
}

glm::vec3 Camera::getFront() const
{
  return front_;
}
/**
 * Asettaa kameran katsomaan annettua kohdetta
 * @param cameraTarget kameran kohdevektori
 */
void Camera::setView(const glm::vec3& cameraTarget)
{
    //view = glm::lookAt(position_, cameraTarget, up_);
    view = glm::lookAt(position_, position_ + front_ , up_);
}


/**
 * Palauttaa kameran sijainnin alkuarvoihin
 */
void Camera::resetView()
{
    position_ = defaultPosition_;
    pitch     =  -45.0;
    yaw       =  -90.0;
    rotateCamera(0,0);
}


/**
 * Vaihtaa kameratyyppiä
 */
void Camera::toggleMode()
{
}


/**
 * Kameran sijainnin muutos
 * @param newPos uusi sijainti
 */
void Camera::translate(const glm::vec3& newPos)
{
    position_ = newPos;
}


/***
 * Kameran suunnan muutos hiirikoordinaattien perusteella.
 * @param xoffset x-muutos
 * @param xoffset y-muutos
 */
void Camera::rotateCamera(const float &xoffset, const float &yoffset)
{
    yaw   += xoffset;
    pitch += yoffset;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    front_.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front_.y = sin(glm::radians(pitch));
    front_.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front_ = glm::normalize(front_);
}


/**
 * Muuttaa kameran liikkumisnopeutta parametrin verran.
 * Minimi 1.0, maksimi 10.0
 * @param adjust muutos
 */
void Camera::adjustSpeed(const float &adjust)
{
    camSpeed += adjust;
    camSpeed = glm::clamp(camSpeed, 1.0f, 10.0f);
}


/**
 * Muuttaa kameran kääntymisnopeutta parametrin verran.
 * Minimi 0.1, maksimi 1.0
 * @param adjust muutos
 */
void Camera::adjustSensitivity(const float &adjust)
{
    camSensitivity += adjust;
    camSensitivity = glm::clamp(camSensitivity, 0.1f, 1.0f);
}


/**
 * Kameran päivitys
 * @param time aikakerroin, (currentTick / lastTick), välitetään program-luokasta
 */
void Camera::update(const float time)
{
        // WASD-kamera
        translate(position_);
        setView(position_ + front_);
}

/**
 * Näppäimistötapahtumien käsittely
 */
void Camera::handleKeyInput()
{
  if (Program::rayCamera == false && pRaycamera) return;
  if (Program::rayCamera == true && !pRaycamera) return;

  auto ic = InputCache::getInstance();
  auto deltaTime2 = ic->get_timeDelta();

  double deltaTime3 = 1.0 + deltaTime2 / 1000000.0;
  this->deltaTime = float(deltaTime3);

  /* Shift hidastaa liikkumisnopeutta */
  float speedMultiplier = camSpeed;
  if (ic->isKeyDown(SDL_SCANCODE_LSHIFT) || ic->isKeyDown(SDL_SCANCODE_RSHIFT) )
  {
      speedMultiplier *= 0.3f;
  }

  if(ic->isKeyDown(SDL_SCANCODE_UP) || ic->isKeyDown(SDL_SCANCODE_W)) {
      position_ += front_ * speedMultiplier * deltaTime;
  }

  if(ic->isKeyDown(SDL_SCANCODE_DOWN) || ic->isKeyDown(SDL_SCANCODE_S))
      position_ -= front_ * speedMultiplier * deltaTime;

  if(ic->isKeyDown(SDL_SCANCODE_LEFT) || ic->isKeyDown(SDL_SCANCODE_A))
      position_ -= glm::normalize(glm::cross(front_, up_)) * speedMultiplier * deltaTime;

  if(ic->isKeyDown(SDL_SCANCODE_RIGHT) || ic->isKeyDown(SDL_SCANCODE_D))
      position_ += glm::normalize(glm::cross(front_, up_)) * speedMultiplier * deltaTime;

  if(ic->isKeyDown(SDL_SCANCODE_V))
      position_ += glm::normalize(up_) * speedMultiplier * deltaTime;

  if(ic->isKeyDown(SDL_SCANCODE_C))
      position_ -= glm::normalize(up_) * speedMultiplier * deltaTime;

  if(ic->isKeyDown(SDL_SCANCODE_O)) {
      if (pFocaldistance < 0.2f) return;
      pFocaldistance -= 0.1f;
      Log::getDebug().log("Focaldistance == %", pFocaldistance);
  }

  if(ic->isKeyDown(SDL_SCANCODE_P)) {
      pFocaldistance += 0.1f;
      Log::getDebug().log("Focaldistance == %", pFocaldistance);
  }
  update(deltaTime);

}

/**
 * Hiiritapahtumien käsittely. Program välittää tapahtumat.
 */
void Camera::handleMouseInput(SDL_Event &inputEvent)
{
  if (Program::rayCamera == false && pRaycamera) return;
  if (Program::rayCamera == true && !pRaycamera) return;
    switch (inputEvent.type)
    {
    case (SDL_MOUSEBUTTONDOWN): /* Klikkaus. Sijainti talteen sulavampaa liikettä varten */
        lastMouseX = inputEvent.motion.x;
        lastMouseY = inputEvent.motion.y;
    case (SDL_MOUSEMOTION):
        switch (inputEvent.motion.state)
        {
        case (SDL_BUTTON_LMASK): /* Hiiren vasen nappi pohjassa */
            rotateCamera((inputEvent.motion.x - lastMouseX) * camSensitivity * deltaTime,
                         (lastMouseY - inputEvent.motion.y) * camSensitivity * deltaTime);
            /* Hiiren sijainti talteen muutoksen laskemista varten */
            lastMouseX = inputEvent.motion.x;
            lastMouseY = inputEvent.motion.y;
            break;
        }
    }
}
