#include <main.h>

SDL_Texture* backgroundOptionScreen = NULL;
SDL_Texture* borderOptionScreen = NULL;
SDL_Texture* imageSource = NULL;
SDL_Texture* easyMode = NULL;
SDL_Texture* mediumMode = NULL;
SDL_Texture* hardMode = NULL;

TTF_Font* gFontOptionScreen = NULL;

void OptionScreen::start(SDL_Renderer* gRenderer, bool& quit) {
    bool nextScreen = false;
    int mode = 0;
    string src = "Picture/default.jpg";

    loadBackground(gRenderer, mode, src, false, "");

    SDL_Event e;
    while (!quit && !nextScreen) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN)
                if( e.button.button == SDL_BUTTON_LEFT ){
                    int x = e.button.x;
                    int y = e.button.y;
                    //left
                    if (173 <= x && x <= 248 && 587 <= y && y <= 629) {
                            mode = (mode - 1 + 3) % 3;
                            loadBackground(gRenderer, mode, src, false, "");
                    }
                    //right
                    if (464 <= x && x <= 540 && 587 <= y && y <= 629) {
                            mode = (mode + 1) % 3;
                            loadBackground(gRenderer, mode, src, false, "");
                    }
                    //image source 137 55 575 518
                    if (137 <= x && x <= 575 && 55 <= y && y <= 518) {
                            changeImageSource(gRenderer, mode, src, quit);
                            loadBackground(gRenderer, mode, src, false, "");
                    }
                }
        }
        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if (currentKeyStates[SDL_SCANCODE_RETURN]) nextScreen = true;
    }

    if (nextScreen) GameScreen::start(gRenderer, quit, mode+2, src);
}

void OptionScreen::changeImageSource(SDL_Renderer* gRenderer, int mode, string& src, bool& quit) {
    bool choosed = false;
    std::string inputText = "";

    loadBackground(gRenderer, mode, src, true, inputText);

    SDL_Event e;
    SDL_StartTextInput();
    while (!quit && !choosed) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN)
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int x = e.button.x;
                    int y = e.button.y;
                    //511 259 553 314
                    if (511 <= x && x <= 553 && 259 <= y && y <= 314) choosed = true;
                }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) inputText.pop_back();
                if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) inputText = SDL_GetClipboardText();
                loadBackground(gRenderer, mode, src, true, inputText);
            }
            if (e.type == SDL_TEXTINPUT)
                if (!(( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C')
                && (e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V') && SDL_GetModState() & KMOD_CTRL)) {
                    inputText += e.text.text;
                    loadBackground(gRenderer, mode, src, true, inputText);
                }
        }
    }
    SDL_StopTextInput();

    if (inputText != "") {
        for (int i = 0; i < inputText.length(); i++)
            if (inputText[i] == '\\')
                inputText[i] = '/';
        if (IMG_Load(inputText.c_str()) != NULL) src = inputText;
    }

    loadBackground(gRenderer, mode, src, false, "");
}

void OptionScreen::loadBackground(SDL_Renderer* gRenderer, int mode, string src, bool isChangingImage, string text) {
    //clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear(gRenderer);

    //load background
    if (backgroundOptionScreen == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/OptionBackground.png");
        backgroundOptionScreen = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }
    SDL_RenderCopy( gRenderer, backgroundOptionScreen, NULL, NULL );

    loadImage(gRenderer, src);

    if (mode == 0) EasyOption(gRenderer);
    if (mode == 1) MediumOption(gRenderer);
    if (mode == 2) HardOption(gRenderer);

    if (isChangingImage) {
        //1 make box
        if (imageSource == NULL) {
            SDL_Surface* surface = IMG_Load("Picture/ImageSource.png");
            imageSource = SDL_CreateTextureFromSurface(gRenderer, surface);
            SDL_FreeSurface(surface);
        }

        SDL_Rect dsrect;
        dsrect.x = 65;
        dsrect.y = 185;
        dsrect.w = 626 - 65;
        dsrect.h = 352 - 185;

        SDL_RenderCopy( gRenderer, imageSource, NULL, &dsrect);

        //make text
        if (text == "") text = " ";
        if (gFontOptionScreen == NULL) gFontOptionScreen = TTF_OpenFont( "score.ttf", 28 );
        SDL_Surface* textSurface = TTF_RenderText_Solid( gFontOptionScreen, text.c_str(), {0,0,0,255} );
        SDL_Texture* mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );

        int mWidth = textSurface->w;
        int mHeight = textSurface->h;
		SDL_FreeSurface( textSurface );
		int blockWidth = min(333, mWidth);

		SDL_Rect srcRect;
		srcRect.x = mWidth - blockWidth;
		srcRect.y = 0;
		srcRect.w = blockWidth;
		srcRect.h = 33;

		SDL_Rect dstRect;
		dstRect.x = 168;
		dstRect.y = 263;
		dstRect.w = blockWidth;
		dstRect.h = 33;

		SDL_RenderCopy(gRenderer, mTexture, &srcRect, &dstRect);
    }

    SDL_RenderPresent( gRenderer );
}

void OptionScreen::loadImage(SDL_Renderer* gRenderer, string src) {
    SDL_Surface* surface = IMG_Load(src.c_str());
    SDL_Texture* background = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dsrect;
    dsrect.x = 170;
    dsrect.y = 85;
    dsrect.w = 375;
    dsrect.h = 405;

    SDL_RenderCopy( gRenderer, background, NULL, &dsrect);

    if (borderOptionScreen == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/border.png");
        borderOptionScreen = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }

    dsrect.x = 167;
    dsrect.y = 79;
    dsrect.w = 382;
    dsrect.h = 417;

    SDL_RenderCopy( gRenderer, borderOptionScreen, NULL, &dsrect);
}

void OptionScreen::EasyOption(SDL_Renderer* gRenderer) {
    if (easyMode == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/EasyMode.png");
        easyMode = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }

    SDL_Rect dsrect;
    dsrect.x = 285;
    dsrect.y = 560;
    dsrect.w = 154;
    dsrect.h = 107;

    SDL_RenderCopy( gRenderer, easyMode, NULL, &dsrect);
}

void OptionScreen::MediumOption(SDL_Renderer* gRenderer) {
    if (mediumMode == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/MediumMode.png");
        mediumMode = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }

    SDL_Rect dsrect;
    dsrect.x = 270;
    dsrect.y = 590;
    dsrect.w = 176;
    dsrect.h = 35;

    SDL_RenderCopy( gRenderer, mediumMode, NULL, &dsrect);
    SDL_RenderPresent( gRenderer );
}

void OptionScreen::HardOption(SDL_Renderer* gRenderer) {
    if (hardMode == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/HardMode.png");
        hardMode = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }

    SDL_Rect dsrect;
    dsrect.x = 305;
    dsrect.y = 590;
    dsrect.w = 107;
    dsrect.h = 34;

    SDL_RenderCopy( gRenderer, hardMode, NULL, &dsrect);
}

//easy 285, 555, 154 ,107
//medium 270, 585, 176, 35
//hard 305, 590, 107 , 34
//image source 65, 185