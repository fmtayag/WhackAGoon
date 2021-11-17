/******************************************************************************

                    TEST FOR SUPERSTATE AND SUBSTATE MACHINES

*******************************************************************************/

#include <iostream>
#include <string>

using namespace std;

class IScene
{
public:
    virtual ~IScene() = default;

    virtual void handleEvents() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};

class SubScene;

class PlayScene : public IScene
{
private:
    enum SubScenesList
    {
        PAUSED,
        TUTORIAL,
        GAMING,
        GAMEOVER
    };
    SubScene *m_subScene;

    friend class PausedSubScene;
    string m_message;

public:
    PlayScene();
    ~PlayScene() override;

    void handleEvents() override;
    void update() override;
    void draw() override;

    void changeSubScene(PlayScene::SubScenesList subscene);
};

class SubScene
{
protected:
    PlayScene *m_parentScene;

public:
    virtual ~SubScene() = default;

    virtual void handleEvents() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};

class PausedSubScene : public SubScene
{
public:
    PausedSubScene(PlayScene *parentScene) { m_parentScene = parentScene; };
    ~PausedSubScene() override{};

    void handleEvents() override
    {
        printf("Paused: handleEvents.\n");
        printf("%s\n", m_parentScene->m_message.c_str());
        printf("Parent scene is: %s.\n", typeid(*m_parentScene).name());
        cout << "Parent Scene address: " << &m_parentScene << endl;
    };
    void update() override { printf("Paused: update.\n"); };
    void draw() override { printf("Paused: draw.\n"); };
};

class GamingSubScene : public SubScene
{
public:
    GamingSubScene(PlayScene *parentScene) { m_parentScene = parentScene; };
    ~GamingSubScene(){};

    void handleEvents() override
    {
        printf("Gaming: handleEvents.\n");
        cout << "Parent Scene address: " << &m_parentScene << endl;
    };
    void update() override { printf("Gaming: update.\n"); };
    void draw() override { printf("Gaming: draw.\n"); };
};

// PlayScene implementation
PlayScene::PlayScene()
{
    changeSubScene(SubScenesList::PAUSED);
    m_message = "Hey there!";
}
PlayScene::~PlayScene()
{
}
void PlayScene::handleEvents()
{
    m_subScene->handleEvents();
}
void PlayScene::update()
{
    m_subScene->update();
}
void PlayScene::draw()
{
    m_subScene->draw();
    changeSubScene(SubScenesList::GAMING);
}
void PlayScene::changeSubScene(PlayScene::SubScenesList subscene)
{
    switch (subscene)
    {
    case SubScenesList::PAUSED:
        delete m_subScene;
        m_subScene = new PausedSubScene(this);
        break;
    case SubScenesList::GAMING:
        delete m_subScene;
        m_subScene = new GamingSubScene(this);
        break;
    default:
        printf("PlayScene::changeSubScene() -> reached default case.\n");
        break;
    }
}

int main()
{

    IScene *curScene = new PlayScene();

    curScene->handleEvents();
    curScene->update();
    curScene->draw(); // change subscene here

    curScene->handleEvents();
    curScene->update();
    curScene->draw();

    return 0;
}
