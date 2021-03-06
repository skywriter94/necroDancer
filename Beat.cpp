#include "stdafx.h"
#include "Beat.h"
#include"gameNode.h"
#include "player.h"
#include "alphaImageEffect.h"

vector<alphaImageEffect*>		     _vEffect;  // 빗나감 이펙트 
vector<alphaImageEffect*>::iterator _viEffect;



HRESULT Beat::init()
{
    // 사운드 추가
    init_AddSoundAndImg();

    // 오브젝트들 초기화
    init_SetObjs();

	_tileOnOff = false;
    return S_OK;
}

void Beat::release()
{
}

void Beat::update()
{
    // 씬 체크
    update_SetSceneMusic();
    
    // 노래 & 노트 조절
    update_SongAndNoteControl();
    
    Move();
    
    // 심장 효과
    update_BeatEffect();
    
    // 거리에 따른 음향 크기 조절
    update_SoundDistanceControl();
    
    // Pitch값 조절
    update_PitchControl();
}

void Beat::render()
{
    //Rectangle(_backBuffer->getMemDC(), heartRC); // 심장 렉트 렌더
    //Rectangle(_backBuffer->getMemDC(), shopKeeperRC);
    IMAGEMANAGER->frameRender("Heart", _backBuffer->getMemDC(), heartImg->getX(), heartImg->getY()); // 심장 렌더
    if (_vNoteLeft.size() > 0) // 왼쪽 노트 렌더
    {
        for (int i = 0; i < _vNoteLeft.size(); i++)
        {
            if (!_vNoteLeft[i].isRender) continue;
            //Rectangle(_backBuffer->getMemDC(), _vNoteLeft[i].rc);
            _vNoteLeft[i].img->alphaRender(_backBuffer->getMemDC(), _vNoteLeft[i].rc.left, _vNoteLeft[i].rc.top, _vNoteLeft[i].alpha);
        }
    }
    if (_vNoteRight.size() > 0) // 오른쪽 노트 렌더
    {
        for (int i = 0; i < _vNoteRight.size(); i++)
        {
            if (!_vNoteRight[i].isRender) continue;
            //Rectangle(_backBuffer->getMemDC(), _vNoteRight[i].rc);
            _vNoteRight[i].img->alphaRender(_backBuffer->getMemDC(), _vNoteRight[i].rc.left, _vNoteRight[i].rc.top, _vNoteRight[i].alpha);
        }
    }

    for (_viEffect = _vEffect.begin(); _viEffect != _vEffect.end(); ++_viEffect) // 이펙트 렌더
    {
        (*_viEffect)->render(_backBuffer->getMemDC());
    }
    IMAGEMANAGER->frameRender("Heart", _backBuffer->getMemDC(), heartImg->getX(), heartImg->getY()); // 심장 렌더
    render_DebugLog(_backBuffer->getMemDC()); // 디버그 텍스트 렌더
}

void Beat::HitNoteEffect(float x, float y) // 플레이어가 노트를 맞힐시 노트가 사라지는 효과를 준다
{
    alphaImageEffect* noteHitEffect;
    noteHitEffect = new alphaImageEffect;
    noteHitEffect->init("GreenNote", x, y, 5, STOP);
    _vEffect.push_back(noteHitEffect);
}



void Beat::init_AddSoundAndImg() // 사운드 & 이미지 추가
{
    // 배경음 관련 (배경음, 상점주인)
    //SOUNDMANAGER->addSound("BGM_LOBBY", "Music/lobby.ogg", false, true);
    SOUNDMANAGER->addSound("BGM_LOBBY", "sound/Music/lobby.ogg", true, false);
    SOUNDMANAGER->addSound("BGM_STAGE_1_1", "sound/Music/zone1_1.ogg", true, false);
    SOUNDMANAGER->addSound("BGM_STAGE_1_2", "sound/Music/zone1_2.ogg", true, false);
    SOUNDMANAGER->addSound("BGM_STAGE_1_3", "sound/Music/zone1_3.ogg", true, false);
    SOUNDMANAGER->addSound("BGM_BOSS", "sound/Music/boss_2.ogg", false, true);

    SOUNDMANAGER->addSound("NPC_Stage_1_1_shopkeeper", "sound/Music/zone1_1_shopkeeper.ogg", true, false);
    SOUNDMANAGER->addSound("NPC_Stage_1_2_shopkeeper", "sound/Music/zone1_2_shopkeeper.ogg", true, false);
    SOUNDMANAGER->addSound("NPC_Stage_1_3_shopkeeper", "sound/Music/zone1_3_shopkeeper.ogg", true, false);

    // SFX 사운드
    SOUNDMANAGER->addSound("sfx_item_food", "sound/ETC/UI/sfx_item_food.ogg", false, false);
    SOUNDMANAGER->addSound("sfx_missedbeat", "sound/ETC/UI/sfx_missedbeat.ogg", false, false);
    SOUNDMANAGER->addSound("sfx_ui_select_down", "sound/ETC/UI/sfx_ui_select_down.ogg", false, false);
    SOUNDMANAGER->addSound("sfx_ui_select_up", "sound/ETC/UI/sfx_ui_select_up.ogg", false, false);
    SOUNDMANAGER->addSound("sfx_pickup_armor", "sound/ETC/UI/sfx_pickup_armor.ogg", false, false);
    SOUNDMANAGER->addSound("sfx_pickup_weapon", "sound/ETC/UI/sfx_pickup_weapon.ogg", false, false);
    SOUNDMANAGER->addSound("sfx_pickup_gold_02", "sound/ETC/UI/sfx_pickup_gold_02.ogg", false, false);
    SOUNDMANAGER->addSound("sfx_pickup_diamond", "sound/ETC/UI/sfx_pickup_diamond.ogg", false, false);
    SOUNDMANAGER->addSound("sfx_pickup_general_ST", "sound/ETC/UI/sfx_pickup_general_ST.ogg", false, false);
    SOUNDMANAGER->addSound("sfx_ui_back", "sound/ETC/UI/sfx_ui_back.ogg", false, false);
    SOUNDMANAGER->addSound("sfx_ui_start", "sound/ETC/UI/sfx_ui_start.ogg", false, false);
    SOUNDMANAGER->addSound("sfx_ui_toggle", "sound/ETC/UI/sfx_ui_toggle.ogg", false, false);

    // 오브젝트 사운드 추가
    SOUNDMANAGER->addSound("obj_door_metal_open", "sound/ETC/Obj/obj_door_metal_open.ogg", false, false);
    SOUNDMANAGER->addSound("obj_door_metal_close", "sound/ETC/Obj/obj_door_metal_close.ogg", false, false);
    SOUNDMANAGER->addSound("obj_door_open", "sound/ETC/Obj/obj_door_open.ogg", false, false);
    SOUNDMANAGER->addSound("obj_trap_bounce", "sound/ETC/Obj/obj_trap_bounce.ogg", false, false);
    SOUNDMANAGER->addSound("obj_trap_trapdoor_fall", "sound/ETC/Obj/obj_trap_trapdoor_fall.ogg", false, false);
    SOUNDMANAGER->addSound("obj_trap_trapdoor_open", "sound/ETC/Obj/obj_trap_trapdoor_open.ogg", false, false);

    // 플레이어 벽 상호작용 사운드
    SOUNDMANAGER->addSound("mov_dig_brick", "sound/player/move/mov_dig_brick.ogg", false, false);
    SOUNDMANAGER->addSound("mov_dig_dirt", "sound/player/move/mov_dig_dirt.ogg", false, false);
    SOUNDMANAGER->addSound("mov_dig_fail", "sound/player/move/mov_dig_fail.ogg", false, false);
    SOUNDMANAGER->addSound("mov_dig_stone", "sound/player/move/mov_dig_stone.ogg", false, false);

    // 플레이어 음성
    SOUNDMANAGER->addSound("vo_cad_death_01", "sound/player/voice/vo_cad_death_01.ogg", false, false);
    SOUNDMANAGER->addSound("vo_cad_dig_01", "sound/player/voice/vo_cad_dig_01.ogg", false, false);
    SOUNDMANAGER->addSound("vo_cad_heal_02", "sound/player/voice/vo_cad_heal_02.ogg", false, false);
    SOUNDMANAGER->addSound("vo_cad_hello_05", "sound/player/voice/vo_cad_hello_05.ogg", false, false);
    SOUNDMANAGER->addSound("vo_cad_hurt_04", "sound/player/voice/vo_cad_hurt_04.ogg", false, false);
    SOUNDMANAGER->addSound("vo_cad_melee_1", "sound/player/voice/vo_cad_melee_1.ogg", false, false);
    SOUNDMANAGER->addSound("vo_cad_melee_2", "sound/player/voice/vo_cad_melee_2.ogg", false, false);
    SOUNDMANAGER->addSound("vo_cad_melee_3", "sound/player/voice/vo_cad_melee_3.ogg", false, false);
    SOUNDMANAGER->addSound("vo_cad_melee_4", "sound/player/voice/vo_cad_melee_4.ogg", false, false);
    SOUNDMANAGER->addSound("vo_cad_trapdoor_01", "sound/player/voice/vo_cad_trapdoor_01.ogg", false, false);
    SOUNDMANAGER->addSound("vo_cad_yeah_02", "sound/player/voice/vo_cad_yeah_02.ogg", false, false);
    
    // 데스메탈 사운드 추가
    SOUNDMANAGER->addSound("deathmetal_intro", "sound/Enemy/boss_Sound/deathmetal_intro.ogg", false, false);
    SOUNDMANAGER->addSound("deathmetal_welcome", "sound/Enemy/boss_Sound/deathmetal_welcome.ogg", false, false);
    SOUNDMANAGER->addSound("deathmetal_hit", "sound/Enemy/boss_Sound/deathmetal_hit.ogg", false, false);
    SOUNDMANAGER->addSound("deathmetal_death", "sound/Enemy/boss_Sound/deathmetal_death.ogg", false, false);
    SOUNDMANAGER->addSound("deathmetal_shieldhit", "sound/Enemy/boss_Sound/deathmetal_shieldhit.ogg", false, false);
    SOUNDMANAGER->addSound("deathmetal_skels", "sound/Enemy/boss_Sound/deathmetal_skels.ogg", false, false);
    SOUNDMANAGER->addSound("deathmetal_skels2", "sound/Enemy/boss_Sound/deathmetal_skels2.ogg", false, false);

    // 인트로 사운드 추가
    SOUNDMANAGER->addSound("Intro", "sound/ETC/Intro/Intro.ogg", false, false);

    // 크레딧 사운드 추가
    SOUNDMANAGER->addSound("credit_music", "sound/ETC/credit/credit_music.mp3", false, false);

    // 이미지 추가
    IMAGEMANAGER->addImage("GreenNote", "image/player/TEMP_beat_marker.bmp", 12, 64, true, RGB(255, 0, 255), true);
    IMAGEMANAGER->addImage("RedNote", "image/player/TEMP_beat_marker_red.bmp", 12, 64, true, RGB(255, 0, 255), true);
    IMAGEMANAGER->addFrameImage("Heart", "image/player/TEMP_beat_heart.bmp", 220, 138, 2, 1, true, RGB(255, 0, 255));
}

void Beat::init_SetObjs() // Beat 클래스에서 제어하고 사용할 여러 변수들 초기화 전용 함수
{
    _currentStage = STAGE_LOBBY;

    _noteFileName = _currentSongName = _oldSongName = ""; // 불러올 파일 이름, 현재 곡 이름, 이전 곡 이름 초기화

    // 초기화(노트 시간 간격, 인풋 시간 간격, 곡의 남은 시간, 심장 프레임 카운팅, 상점 주인 추적 판단 유무, 플레이어 위치 추적 판단 유무, 비트가 울렸는지 판단 유무, 노트 카운팅, 이전 스테이지 인덱스
    // 현재 스테이지 인덱스, 노래 길이, 노래 위치, 노래 pitch값);
     noteTimeIntervalCount = inputIntervalCount = _songLeftTime = heartFrameCount =
     isFindShopkeeperPos = isFindPlayerPos = _isBeating = _countNote = _oldStageID = _currentStageID = _songLength = _songPos = _pitch = 0;

    musicID = 0; // 음악 아이디 값
    musicID_Temp = -1; // 음악 아이디 값 임시 저장

    _pitchTemp = 1; // pitch값 테스팅용

    //test_SlowPlatformPos = { (WINSIZEX / 2) - 100, (WINSIZEY / 2) - 200 };
    //test_slowPlatform = RectMakeCenter(test_SlowPlatformPos.x, test_SlowPlatformPos.y, 48, 48);
    //
    //test_FastPlatformPos = { (WINSIZEX / 2) + 100, (WINSIZEY / 2) - 200 };
    //test_fastPlatform = RectMakeCenter(test_FastPlatformPos.x, test_FastPlatformPos.y, 48, 48);

    heartImg = IMAGEMANAGER->findImage("Heart"); // 심장 렌더
    heartImg->setFrameY(0), heartImg->setFrameX(0);
    heartImg->setX((float)WINSIZEX_HALF - heartImg->getFrameWidth() / 2), heartImg->setY(((float)WINSIZEY - heartImg->getFrameHeight()) - heartImg->getFrameHeight() / 2);
    heartRC = RectMakeCenter(heartImg->getX() + heartImg->getFrameWidth() / 2, heartImg->getY() + heartImg->getFrameHeight() / 2, heartImg->getFrameWidth() + 60, heartImg->getFrameHeight());
}

void Beat::update_SetSceneMusic() // 씬 정보를 받아올 함수
{
    if (musicID == 1) _currentStage = STAGE_LOBBY, _currentStageID = (int)_currentStage, _currentSongName = "BGM_LOBBY", _currentShopkeeper = "", _noteFileName = "sound/Music/lobby.txt", _loopSong = false;
    else if (musicID == 2) _currentStage = STAGE_1_1, _currentStageID = (int)_currentStage, _currentSongName = "BGM_STAGE_1_1", _currentShopkeeper = "NPC_Stage_1_1_shopkeeper", _noteFileName = "sound/Music/zone1_1.txt", _loopSong = false;
    else if (musicID == 3) _currentStage = STAGE_1_2, _currentStageID = (int)_currentStage, _currentSongName = "BGM_STAGE_1_2", _currentShopkeeper = "NPC_Stage_1_2_shopkeeper", _noteFileName = "sound/Music/zone1_2.txt", _loopSong = false;
    else if (musicID == 4) _currentStage = STAGE_1_3, _currentStageID = (int)_currentStage, _currentSongName = "BGM_STAGE_1_3", _currentShopkeeper = "NPC_Stage_1_3_shopkeeper", _noteFileName = "sound/Music/zone1_3.txt", _loopSong = false;
    else if (musicID == 5) _currentStage = BOSS, _currentStageID = (int)_currentStage, _currentSongName = "BGM_BOSS", _currentShopkeeper = "", _noteFileName = "sound/Music/boss_2.txt", _loopSong = true;
}

void Beat::update_PlayerMoveTest() // 테스트용 플레이어
{
    if (KEYMANAGER->isStayKeyDown(VK_UP)) playerPos.y -= 3;
    if (KEYMANAGER->isStayKeyDown(VK_LEFT)) playerPos.x -= 3;
    if (KEYMANAGER->isStayKeyDown(VK_DOWN)) playerPos.y += 3;
    if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) playerPos.x += 3;
    test_Player = RectMakeCenter(playerPos.x, playerPos.y, 25, 25);
}

void Beat::update_SongAndNoteControl() // 곡과 노트 제어
{
    if (TIMEMANAGER->getCountLoadingTime() > 0.5f) // StageScene인 경우에는 로딩이 필요하므로 0.5f의 인터벌을 줌. 혹시 비트 속도가 빨라질 경우 오른쪽 항에 있는 시한 값을 더 늘려야됨.
    {
        SOUNDMANAGER->getSongPosition(_currentSongName, _songPos); // 현재 진행 중인 곡의 진행 시간을 받아 옴(ms)
        _songLeftTime = GetSongVariousTime(_songPos, _songLength); // 현재 곡이 얼만큼 남았는지 알려줌

        // 현재 스테이지가 기존 스테이지와 다를 때 초기화해주는 것들(노트 속도, 남은 시간, 벡터들, 재생 곡 교체, 현재 곡 정보를 담을 키 값들 교체, 델타 타임 다시 받기, 초기 노트 생성)
        if (_currentStageID != _oldStageID)
        {
            Load();
            musicID_Temp = musicID;
            _songLeftTime = 0;
            _vNoteLeft.clear();
            _vNoteRight.clear();
            SOUNDMANAGER->play(_currentSongName), SOUNDMANAGER->play(_currentShopkeeper);
            _oldSongName = _currentSongName;
            _oldShopKeeper = _currentShopkeeper;
            _oldStageID = _currentStageID;
            TIMEMANAGER->setCountTimeResetSwitch(true); // 세는 시간 리셋
            TIMEMANAGER->setCountTimeSwitch(true); // 시간 세기 ON
            _deltaTime = TIMEMANAGER->getElapsedTime();

            CreateNewNote(true);
            CreateNewNote(false);
        }
        TIMEMANAGER->setCountLoadingTimeSwitch(false);
    }

    for (int i = 0; i < _vNoteLeft.size(); i++)
    {
        noteTimeIntervalCount = TIMEMANAGER->getCountTime();
        if (_vNoteLeft.size() < 0)
            break;

        if (noteTimeIntervalCount >= noteTimeInterval)
        {
            TIMEMANAGER->setCountTime(0);
            noteTimeIntervalCount = 0;

            if (_countNote < _vMsTimeInfo.size() - 4) // 노트 생성 수가 노트 정보 길이 - 3보다 작은 경우(노트 개수 오차 보정(맨 처음 노트 스킵한 거 1개, 텍스트에 불러온 끝의 쓰레기 정보 2개만큼 뺌))
            {
                CreateNewNoteWhilePlay(true); // 노트 생성
                CreateNewNoteWhilePlay(false); // 노트 생성
            }
            else // 노트 생성 수가 기존 노트 정보 길이를 넘어갈 때
            {
                if (_loopSong) // 곡이 반복 곡일때
                {
                    _countNote = 0; // 노트를 다시 세기 위해 초기화
                    CreateNewNoteWhilePlay(true); // 노트 생성
                    CreateNewNoteWhilePlay(false); // 노트 생성
                }
            }
        }
        _vNoteRight[i].img->setX(_vNoteRight[i].pos.x - _vNoteRight[i].img->getWidth() / 2); // 이미지 위치 세팅
        _vNoteRight[i].img->setY(_vNoteRight[i].pos.y - _vNoteRight[i].img->getHeight() / 2); // 이미지 위치 세팅
        _vNoteRight[i].rc = RectMakeCenter(_vNoteRight[i].pos.x, _vNoteRight[i].pos.y, _vNoteRight[i].img->getWidth(), _vNoteRight[i].img->getHeight()); // 노트의 렉트 위치 잡기

        _vNoteLeft[i].img->setX(_vNoteLeft[i].pos.x - _vNoteLeft[i].img->getWidth() / 2);
        _vNoteLeft[i].img->setY(_vNoteLeft[i].pos.y - _vNoteLeft[i].img->getHeight() / 2);
        _vNoteLeft[i].rc = RectMakeCenter(_vNoteLeft[i].pos.x, _vNoteLeft[i].pos.y, _vNoteLeft[i].img->getWidth(), _vNoteLeft[i].img->getHeight());

        if (inputIntervalCount > 0.000f)
        {
            Interval = true;
        }
        else Interval = false;

        if (i != 3 && _vNoteLeft[i].alpha < 255)    // 노트 등장시 서서히 알파값 증가 시키기
        {
            _vNoteLeft[i].alpha += 3;
            _vNoteRight[i].alpha += 3;
        }
    }

    // 이펙트, 곡이 끝난 후에 노트 이펙트가 남아 있는 경우를 대비함.
    for (_viEffect = _vEffect.begin(); _viEffect != _vEffect.end(); ++_viEffect)
    {
        (*_viEffect)->update();
    }
}

void Beat::update_BeatEffect() // 심장 박동 시 변경할 심장 이미지와 변경할 이외의 것들을 제어하기 위한 함수
{
    if (_isBeating)
    {
        ++heartFrameCount;
        heartImg->setFrameX(1);
        if (heartFrameCount > HEARTFRAME_RATE)
        {
            heartFrameCount = 0;
            _isBeating = false;
            heartImg->setFrameX(0);
        }
    }
}

void Beat::update_SoundDistanceControl() // 거리에 따라 소리크기 제어
{
    //if (isFindShopkeeperPos) // 상점 주인의 위치를 찾았으면
    //{
    //    shopKeeperRC = RectMakeCenter(shopKeeperPos.x, shopKeeperPos.y, 50, 50);
    //    playerPos = { ((float)_player->getPlayer().rc.right + _player->getPlayer().rc.left) / 2, ((float)_player->getPlayer().rc.bottom + _player->getPlayer().rc.top) / 2 };
    //    float getDistaceTemp = getDistance(playerPos.x, playerPos.y, shopKeeperPos.x, shopKeeperPos.y);
    //    if (getDistaceTemp < WINSIZEX_HALF) SOUNDMANAGER->setVolume(_currentShopkeeper, 1);
    //    else if (getDistaceTemp >= WINSIZEX_HALF && getDistaceTemp < WINSIZEX_HALF * 2) SOUNDMANAGER->setVolume(_currentShopkeeper, 0.75f);
    //    else if (getDistaceTemp >= WINSIZEX_HALF * 2 && getDistaceTemp < WINSIZEX_HALF * 2 + 300) SOUNDMANAGER->setVolume(_currentShopkeeper, 0.30f);
    //    else SOUNDMANAGER->setVolume(_currentShopkeeper, 0.0f);
    //}
    if (isFindShopkeeperPos) // 상점 주인의 위치를 찾았으면
    {
        shopKeeperRC = RectMakeCenter(shopKeeperPos.x, shopKeeperPos.y, 50, 50);
        playerPos = { ((float)_player->getPlayer().rc.right + _player->getPlayer().rc.left) / 2, ((float)_player->getPlayer().rc.bottom + _player->getPlayer().rc.top) / 2 };
        float getDistaceTemp = getDistance(playerPos.x, playerPos.y, shopKeeperPos.x, shopKeeperPos.y);
        if (getDistaceTemp < 200) SOUNDMANAGER->setVolume(_currentShopkeeper, 1);
        else if (getDistaceTemp >= 200 && getDistaceTemp < 400) SOUNDMANAGER->setVolume(_currentShopkeeper, 0.75f);
        else if (getDistaceTemp >= 400 && getDistaceTemp < 600) SOUNDMANAGER->setVolume(_currentShopkeeper, 0.30f);
        else SOUNDMANAGER->setVolume(_currentShopkeeper, 0.0f);
    }
}

void Beat::update_PitchControl() // 곡 속도 제어
{
    //RECT temp;
    //if (IntersectRect(&temp, &test_Player, &test_slowPlatform)) SOUNDMANAGER->setPitch(_currentShopkeeper, 0.875f), SOUNDMANAGER->setPitch(_currentSongName, 0.875f);
    //else if ((IntersectRect(&temp, &test_Player, &test_fastPlatform))) SOUNDMANAGER->setPitch(_currentShopkeeper, 1.125f), SOUNDMANAGER->setPitch(_currentSongName, 1.125f);
    //else SOUNDMANAGER->setPitch(_currentShopkeeper, 1), SOUNDMANAGER->setPitch(_currentSongName, 1);

    //if (KEYMANAGER->isOnceKeyDown('N'))
    //{
    //    _pitchTemp -= 0.1f;
    //    SOUNDMANAGER->setPitch(_currentShopkeeper, _pitchTemp), SOUNDMANAGER->setPitch(_currentSongName, _pitchTemp);
    //}
    //if (KEYMANAGER->isOnceKeyDown('M'))
    //{
    //    _pitchTemp += 0.1f;
    //    SOUNDMANAGER->setPitch(_currentShopkeeper, _pitchTemp), SOUNDMANAGER->setPitch(_currentSongName, _pitchTemp);
    //}
}

void Beat::render_DebugLog(HDC getMemDC) // 디버그용 함수
{
    //char display_Pitch[128];
    //sprintf_s(display_Pitch, sizeof(display_Pitch), "현재 pitch값 : %f", _pitchTemp);
    //TextOut(getMemDC, 100, 100, display_Pitch, strlen(display_Pitch));
    //if (isFindShopkeeperPos)
    //{
    //    char display_Pitch[128];
    //    sprintf_s(display_Pitch, sizeof(display_Pitch), "%d", shopKeeperPos.x);
    //    TextOut(getMemDC, 100, 100, display_Pitch, strlen(display_Pitch));
    //
    //    char display_countNote[256];
    //    sprintf_s(display_countNote, sizeof(display_countNote), "%d", shopKeeperPos.y);
    //    TextOut(getMemDC, 100, 120, display_countNote, strlen(display_countNote));
    //}
    //if (_currentSongName != "")
    //{
    //
    //    char display_inputIntervalCount[256];
    //    sprintf_s(display_inputIntervalCount, sizeof(display_inputIntervalCount), "%f", inputIntervalCount);
    //    TextOut(getMemDC, 100, 140, display_inputIntervalCount, strlen(display_inputIntervalCount));
    //
    //    char display_songLength[256];
    //    sprintf_s(display_songLength, sizeof(display_songLength), "%d", _oldStageID);
    //    TextOut(getMemDC, 100, 180, display_songLength, strlen(display_songLength));
    //
    //    char display_checkInfo[256];
    //    sprintf_s(display_checkInfo, sizeof(display_checkInfo), "%d", _currentStageID);
    //    TextOut(getMemDC, 100, 200, display_checkInfo, strlen(display_checkInfo));
    //
    //    char display_checkAverSpeed[256];
    //    sprintf_s(display_checkAverSpeed, sizeof(display_checkAverSpeed), "(_vMsTimeInfo[i + 1] - _vMsTimeInfo[i]) / 1000.0f : %f", (_vMsTimeInfo[_countNote + 1] - _vMsTimeInfo[_countNote]) / 1000.0f);
    //    TextOut(getMemDC, 100, 220, display_checkAverSpeed, strlen(display_checkAverSpeed));
    //}
}

void Beat::Load() // 노트 파일 로드
{
    if (_vMsTimeInfo.size() > 0)
    {
        _vMsTimeInfo.clear(); // 벡터 재사용을 위한 클리어
        _countNote = 0;
    }
    _songLength = 0; // 곡의 길이 값 초기화

    ifstream readFile;
    string tempWord;
    readFile.open(_noteFileName); // 파일 열기

    if (readFile.is_open()) // 파일이 정상적으로 열려있다면
    {
        while (!readFile.eof()) // 읽어오는 파일의 끝을 만날때까지 반복
        {
            char temp; // 문자를 하나씩 뽑기 위한 변수
            readFile.get(temp); // 문자 하나 뽑기 
            if (temp == ',')
            {
                _vMsTimeInfo.push_back(atoi(tempWord.c_str())); // 변환 순서 : 문자열 -> char*로 변환 -> atoi는 char*을 int로 변환
                tempWord = ""; // string 초기화
                continue;
            }
            tempWord += temp; // 한 글자씩 string에 합침
        }
        readFile.close(); // 파일 닫기
    }
    // FMOD::SOUND에 getLength() 함수가 망가져서 만들었음 ㅠㅠ... 아마 원인이 함수도 음악 파일 확장자에 따라서 잘 작동되는 함수가 있는 반면 아닌 함수도 있다. 예를 들어 getVolume함수 같은 경우에도 일부 확장자만 함수 사용이 가능함
    for (int i = 0; i < _vMsTimeInfo.size() - 2; i++)
    {
        int getMS = _vMsTimeInfo[i + 1] - _vMsTimeInfo[i]; // 한 개의 비트 간격을 받아오기 위해서..
        _songLength += getMS; // 길이를 담아준다. 나중엔 _songLength가 곡의 총 길이가 된다.
    }
}

void Beat::CreateNewNote(bool dirRight) // 노트 생성, 곡 시작 직전에 (오른쪽으로 생성할 건지 말건지 정하기)
{
    for (int i = 0; i < 3; i++) // 새 노트로 리셋, 오른쪽
    {
        tagNote setNote;
        int tempMS = _vMsTimeInfo[_countNote + 1] - _vMsTimeInfo[_countNote];
        int bpm = 60000 / tempMS;

        if (dirRight) // 방향이 오른쪽인 경우
        {
            setNote.pos = { (float)WINSIZEX_HALF + (NOTE_INTERVAL * (i + 1)), (float)(heartRC.top + heartRC.bottom) / 2 };
            setNote.speed = lerp(setNote.pos.x, setNote.pos.x - NOTE_INTERVAL, (_deltaTime / ((tempMS + bpm) / 1000.0f)));
        }
        else // 방향이 왼쪽인 경우
        {
            setNote.pos = { (float)WINSIZEX_HALF - (NOTE_INTERVAL * (i + 1)), (float)(heartRC.top + heartRC.bottom) / 2 };
            setNote.speed = lerp(setNote.pos.x, setNote.pos.x + NOTE_INTERVAL, (_deltaTime / ((tempMS + bpm) / 1000.0f)));
        }

        setNote.img = new image;
        setNote.img = IMAGEMANAGER->findImage("GreenNote");
        setNote.img->setX(setNote.pos.x - setNote.img->getWidth() / 2);
        setNote.img->setY(setNote.pos.y - setNote.img->getHeight() / 2);
        setNote.rc = RectMakeCenter(setNote.pos.x, setNote.pos.y, setNote.img->getWidth(), setNote.img->getHeight());
        setNote.isCol = false;
        setNote.isRender = true;
		setNote.colision = false;
        setNote.alpha = 127;

        noteTimeInterval = (_vMsTimeInfo[_countNote + 1] - _vMsTimeInfo[_countNote]) / 1000.0f;

        if (dirRight) _vNoteRight.push_back(setNote);
        else _vNoteLeft.push_back(setNote), ++_countNote; // *카운트 노트는 왼쪽 기준으로만 세주자! 양쪽다 하면 두 번 일하는거니깐!
    }
}

void Beat::CreateNewNoteWhilePlay(bool dirRight) // 노트 생성, 곡 시작 중 (오른쪽으로 생성할 건지 말건지 정하기)
{
    tagNote newNote;
    int tempMS = _vMsTimeInfo[_countNote + 1] - _vMsTimeInfo[_countNote];
    int bpm = 60000 / tempMS;

    if (dirRight) // 방향이 오른쪽인 경우
    {
        newNote.pos = { WINSIZEX, (float)(heartRC.top + heartRC.bottom) / 2 };
    }
    else // 방향이 왼쪽인 경우
    {
        newNote.pos = { 0, (float)(heartRC.top + heartRC.bottom) / 2 };
    }
    newNote.speed = lerp(newNote.pos.x, WINSIZEX_HALF, (_deltaTime / ((tempMS + bpm) / 1000.0f)) / 3);

    if (_loopSong) newNote.img = IMAGEMANAGER->findImage("GreenNote"); // 반복 곡인 경우에는 계속 초록색 노트 이미지를 지정해준다.
    else if (_songLeftTime > (float)(_songLength / 1000) * 0.15f && !_loopSong) newNote.img = IMAGEMANAGER->findImage("GreenNote"); // 현재 남은 곡의 전체 곡 길이의 15%보다 큰 경우 초록색 노트 이미지 지정 
    else newNote.img = IMAGEMANAGER->findImage("RedNote"); // 그렇지 않다면 빨간 노트로 지정
    newNote.img->setX(newNote.pos.x - newNote.img->getWidth() / 2);
    newNote.img->setY(newNote.pos.y - newNote.img->getHeight() / 2);

    newNote.rc = RectMakeCenter(newNote.pos.x, newNote.pos.y, newNote.img->getWidth(), newNote.img->getHeight());
    newNote.isCol = false;
    newNote.isRender = true;
	newNote.colision = false;
    newNote.alpha = 127;

    noteTimeInterval = (_vMsTimeInfo[_countNote + 1] - _vMsTimeInfo[_countNote]) / 1000.0f;

    if (dirRight) _vNoteRight.push_back(newNote);
    else _vNoteLeft.push_back(newNote), ++_countNote; // 노트를 셀 때, 양쪽 다 세면 값이 2씩 증가하므로 한 쪽만 세준다.
}

float Beat::GetSongVariousTime(unsigned int playTime, unsigned int songLength)
{
    float songLeftLength = songLength - playTime;
    songLeftLength /= 1000;
    return songLeftLength;
}

void Beat::Move()
{
    if (_songPos > 0) // 곡이 시작되었을때
    {
        for (int i = 0; i < _vNoteLeft.size(); i++)
        {
            _vNoteRight[i].pos.x += _vNoteRight[i].speed * SOUNDMANAGER->getPitch(_currentSongName, _pitch);
            _vNoteLeft[i].pos.x += _vNoteLeft[i].speed * SOUNDMANAGER->getPitch(_currentSongName, _pitch);

            RECT temp;
            if (IntersectRect(&temp, &_vNoteLeft[i].rc, &heartRC)) // 노트가 심장이랑 충돌 시 
            {
				// Tile 색깔 턴오프
                if (!_vNoteLeft[i].isCol) _isBeating = true; // 심장 이미지 변경을 위해 true로 변경
                _vNoteLeft[i].isCol = true; // 인터벌이 다 지나고 두 번 심장이 두근거리는 것을 방지하기 위해 true로 변경
                inputIntervalCount += _deltaTime; // 입력할 수 있는 시간 구함

                 // 플레이어가 키 입력시
                if (_player->getPlayerKey() && Interval)
                {
                    _player->setPlayerKey();
					_vNoteLeft[i].colision = true;
                    if (_vNoteLeft[i].pos.x < WINSIZEX_HALF)
                    {
                        HitNoteEffect(_vNoteLeft[i].pos.x - NOTE_RADIUS_X, ((heartRC.bottom + heartRC.top) / 2) - ((_vNoteLeft[i].rc.bottom - _vNoteLeft[i].rc.top) / 2));
                        _vNoteLeft[i].isRender = false;
                        _effect = true;

                        HitNoteEffect(_vNoteRight[i].pos.x - NOTE_RADIUS_X, ((heartRC.bottom + heartRC.top) / 2) - ((_vNoteRight[i].rc.bottom - _vNoteRight[i].rc.top) / 2));
                        _vNoteRight[i].isRender = false;
                        _effect = true;
                    }
                    inputIntervalCount = 0;
                }
            }
            if (_vNoteLeft[i].pos.x > WINSIZEX_HALF) 
            {
                _vNoteLeft[i].isRender = false;
                _vNoteRight[i].isRender = false;
            }
            if (_vNoteLeft[i].pos.x - (heartImg->getFrameWidth() / 2) > WINSIZEX_HALF)
            {
                _player->setPlayerKey();
				_player->setPlayerKeyDown();
				if (_tileOnOff)
				{
					_tileOnOff = false;
				}
				else
				{
					_tileOnOff = true;
				}
                inputIntervalCount = 0;
				if (!_vNoteLeft[i].colision)
				{
					_player->setCombo();
				}
                _vNoteRight.erase(_vNoteRight.begin() + i);
                _vNoteLeft.erase(_vNoteLeft.begin() + i);
                break;
            }
        }
    }
}

void Beat::AllStopMusic()
{
    // Music
    SOUNDMANAGER->stop("BGM_LOBBY");
    SOUNDMANAGER->stop("BGM_STAGE_1_1");
    SOUNDMANAGER->stop("BGM_STAGE_1_2");
    SOUNDMANAGER->stop("BGM_STAGE_1_3");
    SOUNDMANAGER->stop("BGM_BOSS");

    // Shop Keeper
    SOUNDMANAGER->stop("NPC_Stage_1_1_shopkeeper");
    SOUNDMANAGER->stop("NPC_Stage_1_2_shopkeeper");
    SOUNDMANAGER->stop("NPC_Stage_1_3_shopkeeper");

    // Intro
    SOUNDMANAGER->stop("Intro");

    // Credit
    SOUNDMANAGER->stop("credit_music");
}