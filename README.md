# <mark> Recreating Persona5 UI System </mark>
Recreating Persona5 UI System은 Persona 5의 시각적 연출과 인터랙션 구조를 분석한 뒤,<br>
언리얼 엔진 5.6의 **Common UI 시스템**을 기반으로 재현한 프로젝트입니다.<br>
UI 스택 구조, 입력 매핑, 애니메이션 처리 등 UI 시스템을 직접 설계하고 구현했습니다.

## <mark> 영상 보기 </mark>
🎥 아래의 사진을 클릭시 구현 동영상을 보실 수 있습니다.

[![Recreating Persona5 UI System](https://img.youtube.com/vi/qd9V9iDbybs/0.jpg)](https://youtu.be/qd9V9iDbybs)

<br>

## <mark> 사용 기술 </mark>
- Unreal Engine 5.5
- C++ / Blueprint 혼합 구조
- Rider

<br>

## <mark> 프로젝트 개요 </mark>
| 항목 | 내용 |
| --- | --- |
| **장르** | JRPG |
| **플랫폼** | PC |
| **개발 내용** | 플레이어, 전투 피드백 UI, 무기별 공격 로직, 무기 슬롯 시스템 |

<br>

# <mark> 구현 상세 설명 </mark>
구현 기능의 소스 코드는 각 소제목 밑의 링크를 통해 확인하실 수 있습니다.

---
## <mark> Common UI 구조 설명 </mark>
> [GameplayTag 선언 소스코드](https://github.com/kimskyeee/Recreating-Persona5/blob/master/Source/UIProject/GameplayTag/UIGameplayTagInfo.h)<br>
> [RootWidget::PushByTag 소스코드](https://github.com/kimskyeee/Recreating-Persona5/blob/master/Source/UIProject/MainMenuUI/RootWidget.cpp#L83C1-L108C2)

Common UI는 `RootWidget` → 4개 스택(Main/Overlay/Modal/HUD) 구조로 설계했으며,<br>
GameplayTag 기반으로 Push/Pop 하며 화면을 관리합니다.<br>
모든 UI의 기본 베이스인 `UBaseWidget`에 ESC 등 글로벌 입력을 상주시켜 전역 ESC 입력을 처리했습니다. <br>
태그는 레이어(스택)·스크린(화면표시)을 명확히 분리했습니다.<br>

<img width="1519" height="1233" alt="image" src="https://github.com/user-attachments/assets/6c27fed3-6b44-4e1c-b6aa-91ec086ef8b2" />

## <mark> 1. 게임 로비 화면 구성 </mark>
### <mark> Anim Notify를 활용한 착지 연출 </mark>
> [MainMenuPawn 소스코드](https://github.com/kimskyeee/Recreating-Persona5/blob/master/Source/UIProject/MainMenuPawn/MainMenuPawnChar.cpp#L96C1-L142C2)

<img width="873" height="307" alt="image" src="https://github.com/user-attachments/assets/42c784a3-f53a-4b9b-ae1e-a83a0c7d30e5" />

- 게임 시작 메뉴 진입 시 캐릭터가 하늘에서 착지해 일어서는 연출을 Anim Notify로 제어했습니다.
- StartPose → LandLoop → Stand 세 구간으로 나누어, 착지 루프 시점(AnimNotify_OnEnteredLandLoop)에서 잠시 정지 후 
- 카메라 전환과 Press Any Key UI가 자연스럽게 등장하도록 설계했습니다.

<br>

### <mark> UPressAnyKeyWidget </mark>
> [PressAnyKey 소스코드](https://github.com/kimskyeee/Recreating-Persona5/blob/master/Source/UIProject/MainMenuUI/PressAnyKeyWidget.cpp#L20C1-L45C2)

https://github.com/user-attachments/assets/92b4f146-6fb4-41e5-8e43-42e585de24ba

- 게임 진입 시 표시되는 Press Any Key 화면은 키보드·마우스 입력을 모두 감지하여 첫 입력 순간에 메인 메뉴로 자연스럽게 전환됩니다.
- `NativeOnKeyDown`과 `NativeOnMouseButtonDown`을 오버라이드하여 모든 유효 입력을 즉시 처리(FReply::Handled) 하고, `GoMainMenu()`가 호출되도록 설계했습니다.

<br>

### <mark> 메뉴 호버시 카메라 전환과 전광판 효과 </mark>
> [PlayerController 카메라 전환 소스코드](https://github.com/kimskyeee/Recreating-Persona5/blob/master/Source/UIProject/UIProjectPlayerController.cpp#L197C1-L215C2) <br>
> [Main Menu 카메라 처리 소스코드](https://github.com/kimskyeee/Recreating-Persona5/blob/master/Source/UIProject/MainMenuUI/MainMenuWidget.cpp#L77C1-L100C2)<br>
> [전광판 연출(AABillboard) 소스코드](https://github.com/kimskyeee/Recreating-Persona5/blob/master/Source/UIProject/MainMenuUI/UIMaterial/ABillboard.cpp#L30C1-L74C2)

https://github.com/user-attachments/assets/8dcd2068-99ec-47fa-9c63-e79bd1d41bf5

- 전광판(AABillboard)은 자신의 화면을 실시간으로 캡처해 표시하기 위해
- 두 개의 `UTextureRenderTarget2D`를 번갈아 사용하는 **핑퐁(Render Target Ping-Pong) 구조**로 설계되었습니다.
- 렌더 타겟 2개(RT[0], RT[1])를 생성한 뒤, 한쪽은 캡처용(Write), 다른 한쪽은 화면 표시용(Read) 으로 분리하여 매 프레임 교체(Swap)합니다.
- 이를 통해 자기 자신을 렌더링할 때 발생하는 피드백 루프(recursive rendering) 문제를 방지했습니다.

<br>

---

## <mark> 2. 인게임 Minimap UI 시스템 </mark>

https://github.com/user-attachments/assets/c63b6bf4-99bb-479b-a940-0a65afe07ac6

- 플레이어의 현재 위치와 방향을 실시간으로 반영하며, 이동 경로를 발자국 형태로 시각화하는 동적 미니맵입니다.
- 언리얼 엔진의 `UCommonUserWidget`을 기반으로, Zoom(확대/축소), 월드 좌표 정규화, 발자국 풀 관리(Pooling) 등의 로직을 포함하고 있습니다.

### <mark> 지도 스크롤링 </mark>
> [지도 및 플레이어마커 회전 소스코드](https://github.com/kimskyeee/Recreating-Persona5/blob/master/Source/UIProject/InGameHUD/MapWidget.cpp#L61C1-L97C2)

- 플레이어의 월드 좌표를 지도 UV(0~1) 공간으로 변환하여 표시합니다.
- 플레이어의 움직임에 따라 지도가 플레이어와 반대 방향으로 스크롤링되므로, 화면 중앙에는 항상 캐릭터가 고정됩니다.
- Zoom 비율에 따라 중심점(U=0.5, V=0.5) 주위로 맵이 이동 → UOffset, VOffset 계산으로 화면 중앙 정렬 유지 → 결과적으로 플레이어는 고정되고 맵이 반대 방향으로 스크롤

### <mark> 발자국 스크롤링 </mark>
> [발자국 소스코드](https://github.com/kimskyeee/Recreating-Persona5/blob/master/Source/UIProject/InGameHUD/MapWidget.cpp#L121C1-L198C2)

- 플레이어가 일정 거리(StepDistanceWorld)를 이동할 때마다 발자국을 생성합니다.
- 외적을 통한 좌우 교차 로직을 적용하여, 실제 캐릭터의 걷기 리듬에 맞는 시각적 패턴을 구현했습니다.
- 발자국은 8개의 이미지풀링으로 관리되며, 3초 이후 페이드 아웃됩니다.

<br>

---

## <mark> 3. Common UI를 활용한 전역 ESC 시스템 </mark>
> [전역 ESC 소스코드](https://github.com/kimskyeee/Recreating-Persona5/blob/master/Source/UIProject/MainMenuUI/BaseWidget.cpp#L12C1-L31C2)

https://github.com/user-attachments/assets/207c869c-40ec-4cd3-bd3b-5ac0e08637e9

- `UBaseWidget`은 CommonUI 기반 모든 UI의 공통 부모로, ESC 키를 전역 메뉴 토글로 처리합니다.
- `IA_Escape` 입력을 바인딩한 후 최상위 `URootWidget`의 `OnHandleEscape()`를 호출해 어떤 화면에서도 메뉴를 열거나 닫을 수 있습니다.

