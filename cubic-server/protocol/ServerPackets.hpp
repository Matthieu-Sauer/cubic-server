#ifndef AE9C1FA0_D3A2_4B7D_962E_4EAF72963603
#define AE9C1FA0_D3A2_4B7D_962E_4EAF72963603

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "PlayerAttributes.hpp"
#include "protocol/Structures.hpp"
#include "typeSerialization.hpp"
#include "types.hpp"

namespace protocol {

enum class ServerPacketsID : int32_t {
    Handshake = 0x00,

    // Status state
    StatusRequest = 0x00,
    PingRequest = 0x01,

    // Login state
    LoginStart = 0x00,
    EncryptionResponse = 0x01,

    // Play state
    ConfirmTeleportation = 0x00,
    QueryBlockEntityTag = 0x01,
    ChangeDifficulty = 0x02,
    MessageAcknowledgement = 0x03,
    ChatCommand = 0x04,
    ChatMessage = 0x05,
    ClientCommand = 0x06,
    ClientInformation = 0x07,
    CommandSuggestionRequest = 0x08,
    ClickContainerButton = 0x09,
    ClickContainer = 0x0a,
    CloseContainerRequest = 0x0b,
    PluginMessage = 0x0c,
    EditBook = 0x0d,
    QueryEntityTag = 0x0e,
    Interact = 0x0F,
    JigsawGenerate = 0x10,
    KeepAliveResponse = 0x11,
    LockDifficulty = 0x12,
    SetPlayerPosition = 0x13,
    SetPlayerPositionAndRotation = 0x14,
    SetPlayerRotation = 0x15,
    SetPlayerOnGround = 0x16,
    MoveVehicle = 0x17,
    PaddleBoat = 0x18,
    PickItem = 0x19,
    PlaceRecipe = 0x1a,
    PlayerAbilities = 0x1b,
    PlayerAction = 0x1c,
    PlayerCommand = 0x1d,
    PlayerInput = 0x1e,
    Pong = 0x1f,
    PlayerSession = 0x20,
    ChangeRecipeBookSettings = 0x21,
    SetSeenRecipe = 0x22,
    RenameItem = 0x23,
    ResourcePack = 0x24,
    SeenAdvancements = 0x25,
    SelectTrade = 0x26,
    SetBeaconEffect = 0x27,
    SetHeldItem = 0x28,
    ProgramCommandBlock = 0x29,
    ProgramCommandBlockMinecart = 0x2a,
    SetCreativeModeSlot = 0x2b,
    ProgramJigsawBlock = 0x2c,
    ProgramStructureBlock = 0x2d,
    UpdateSign = 0x2e,
    SwingArm = 0x2f,
    TeleportToEntity = 0x30,
    UseItemOn = 0x31,
    UseItem = 0x32,
};

struct BaseServerPacket { };

// Packets

struct Handshake : BaseServerPacket {
    int32_t prot_version;
    std::string addr;
    uint16_t port;
    enum class State : int32_t {
        Status = 1,
        Login = 2,
    } next_state;
};
std::shared_ptr<Handshake> parseHandshake(std::vector<uint8_t> &buffer);

struct StatusRequest : BaseServerPacket { };
std::shared_ptr<StatusRequest> parseStatusRequest(std::vector<uint8_t> &buffer);

struct PingRequest : BaseServerPacket {
    int64_t payload;
};
std::shared_ptr<PingRequest> parsePingRequest(std::vector<uint8_t> &buffer);

struct LoginStart : BaseServerPacket {
    std::string name;
    bool has_player_uuid;
    u128 player_uuid;
};
std::shared_ptr<LoginStart> parseLoginStart(std::vector<uint8_t> &buffer);

struct EncryptionResponse : BaseServerPacket {
    std::vector<uint8_t> shared_secret;
    bool has_verify_token;
    std::vector<uint8_t> verify_token;
    int64_t salt;
    std::vector<uint8_t> message_signature;
};
std::shared_ptr<EncryptionResponse> parseEncryptionResponse(std::vector<uint8_t> &buffer);

struct ConfirmTeleportation : BaseServerPacket {
    int32_t teleport_id;
};
std::shared_ptr<ConfirmTeleportation> parseConfirmTeleportation(std::vector<uint8_t> &buffer);

struct QueryBlockEntityTag : BaseServerPacket {
    int32_t transaction_id;
    Position location;
};
std::shared_ptr<QueryBlockEntityTag> parseQueryBlockEntityTag(std::vector<uint8_t> &buffer);

struct ChangeDifficulty : BaseServerPacket {
    player_attributes::Gamemode new_difficulty;
};
std::shared_ptr<ChangeDifficulty> parseChangeDifficulty(std::vector<uint8_t> &buffer);

struct MessageAcknowledgement : BaseServerPacket {
    int32_t message_count;
};
std::shared_ptr<MessageAcknowledgement> parseMessageAcknowledgement(std::vector<uint8_t> &buffer);

/**
 * @brief this is the link to the packet: https://wiki.vg/Protocol#Chat_Command
 *
 */
struct ChatCommand : BaseServerPacket {
    std::string command;
    long timestamp;
    long salt;
    std::vector<ArgumentSignature> argumentSignatures;
    int32_t messageCount;
    std::bitset<20> acknowledged;
};

/**
 * @brief This function is used to parse the chat command packet
 *
 * @param buffer
 * @return std::shared_ptr<ChatCommand>
 */
std::shared_ptr<ChatCommand> parseChatCommand(std::vector<uint8_t> &buffer);

struct ChatMessage : BaseServerPacket {
    std::string message;
    // I think this is a string, it is marked as an 'Instant' in the protocol
    // https://wiki.vg/index.php?title=Protocol&oldid=17753#Chat_Command
    Instant timestamp;
    long salt;
    std::vector<uint8_t> signature;
    bool isSigned;
};
std::shared_ptr<ChatMessage> parseChatMessage(std::vector<uint8_t> &buffer);

struct ClientCommand : BaseServerPacket {
    enum class ActionID : int32_t {
        PerformRespawn = 0,
        RequestStats = 1,
    } action_id;
};
std::shared_ptr<ClientCommand> parseClientCommand(std::vector<uint8_t> &buffer);

struct ClientInformation : BaseServerPacket {
    std::string locale;
    uint8_t view_distance;
    enum class ChatMode : int32_t {
        Enabled = 0,
        CommandsOnly = 1,
        Hidden = 2,
    } chat_mode;
    bool chat_colors;
    uint8_t displayed_skin_parts;
    enum class MainHand : int32_t {
        Left = 0,
        Right = 1,
    } main_hand;
    bool enable_text_filtering;
    bool allow_server_listings;
};
std::shared_ptr<ClientInformation> parseClientInformation(std::vector<uint8_t> &buffer);

struct CommandSuggestionRequest : BaseServerPacket {
    int32_t transaction_id;
    std::string text;
};
std::shared_ptr<CommandSuggestionRequest> parseCommandSuggestionRequest(std::vector<uint8_t> &buffer);

struct ClickContainerButton : BaseServerPacket {
    uint8_t window_id;
    uint8_t button_id;
};
std::shared_ptr<ClickContainerButton> parseClickContainerButton(std::vector<uint8_t> &buffer);

struct ClickContainer : BaseServerPacket {
    uint8_t window_id;
    int32_t state_id;
    int16_t slot;
    int8_t button;
    int32_t mode; // May be an enum but there is to many cases
    std::vector<SlotWithIndex> array_of_slots;
    Slot carried_item;
};
std::shared_ptr<ClickContainer> parseClickContainer(std::vector<uint8_t> &buffer);

struct CloseContainerRequest : BaseServerPacket {
    uint8_t window_id;
};
std::shared_ptr<CloseContainerRequest> parseCloseContainerRequest(std::vector<uint8_t> &buffer);

struct PluginMessage : BaseServerPacket {
    std::string channel;
    std::vector<uint8_t> data;
};
std::shared_ptr<PluginMessage> parsePluginMessage(std::vector<uint8_t> &buffer);

struct EditBook : BaseServerPacket {
    int32_t slot;
    std::vector<std::string> entries;
    bool has_title;
    std::string title;
};
std::shared_ptr<EditBook> parseEditBook(std::vector<uint8_t> &buffer);

struct QueryEntityTag : BaseServerPacket {
    int32_t transaction_id;
    int32_t entity_id;
};
std::shared_ptr<QueryEntityTag> parseQueryEntityTag(std::vector<uint8_t> &buffer);

struct Interact : BaseServerPacket {
    int32_t entity_id;
    enum class Type : int32_t {
        Interact = 0,
        Attack = 1,
        InteractAt = 2,
    } type;
    float target_x;
    float target_y;
    float target_z;
    enum class Hand : int32_t {
        MainHand = 0,
        OffHand = 1,
    } hand;
    bool sneaking;
};
std::shared_ptr<Interact> parseInteract(std::vector<uint8_t> &buffer);

struct JigsawGenerate : BaseServerPacket {
    Position location;
    int32_t levels;
    bool keep_jigsaws;
};
std::shared_ptr<JigsawGenerate> parseJigsawGenerate(std::vector<uint8_t> &buffer);

struct KeepAliveResponse : BaseServerPacket {
    int64_t keep_alive_id;
};
std::shared_ptr<KeepAliveResponse> parseKeepAliveResponse(std::vector<uint8_t> &buffer);

struct LockDifficulty : BaseServerPacket {
    bool locked;
};
std::shared_ptr<LockDifficulty> parseLockDifficulty(std::vector<uint8_t> &buffer);

struct SetPlayerPosition : BaseServerPacket {
    double x;
    double feet_y;
    double z;
    bool on_ground;
};
std::shared_ptr<SetPlayerPosition> parseSetPlayerPosition(std::vector<uint8_t> &buffer);

struct SetPlayerPositionAndRotation : BaseServerPacket {
    double x;
    double feet_y;
    double z;
    float yaw;
    float pitch;
    bool on_ground;
};
std::shared_ptr<SetPlayerPositionAndRotation> parseSetPlayerPositionAndRotation(std::vector<uint8_t> &buffer);

struct SetPlayerRotation : BaseServerPacket {
    float yaw;
    float pitch;
    bool on_ground;
};
std::shared_ptr<SetPlayerRotation> parseSetPlayerRotation(std::vector<uint8_t> &buffer);

struct SetPlayerOnGround : BaseServerPacket {
    bool on_ground;
};
std::shared_ptr<SetPlayerOnGround> parseSetPlayerOnGround(std::vector<uint8_t> &buffer);

struct MoveVehicle : BaseServerPacket {
    double x;
    double y;
    double z;
    float yaw;
    float pitch;
};
std::shared_ptr<MoveVehicle> parseMoveVehicle(std::vector<uint8_t> &buffer);

struct PaddleBoat : BaseServerPacket {
    bool left_paddle_turning;
    bool right_paddle_turning;
};
std::shared_ptr<PaddleBoat> parsePaddleBoat(std::vector<uint8_t> &buffer);

struct PickItem : BaseServerPacket {
    int32_t slot_to_use;
};
std::shared_ptr<PickItem> parsePickItem(std::vector<uint8_t> &buffer);

struct PlaceRecipe : BaseServerPacket {
    uint8_t window_id;
    std::string recipe;
    bool make_all;
};
std::shared_ptr<PlaceRecipe> parsePlaceRecipe(std::vector<uint8_t> &buffer);

struct PlayerAbilities : BaseServerPacket {
    enum Flags : uint8_t {
        Invulnerable = 0x01,
        Flying = 0x02,
        AllowFlying = 0x04,
        CreativeMode = 0x08
    };
    uint8_t flags;
};
std::shared_ptr<PlayerAbilities> parsePlayerAbilities(std::vector<uint8_t> &buffer);

struct PlayerAction : BaseServerPacket {
    enum class Status : int32_t {
        StartedDigging = 0,
        CancelledDigging = 1,
        FinishedDigging = 2,
        DropItemStack = 3,
        DropItem = 4,
        ShootArrowOrFinishEating = 5,
        SwapItemInHand = 6,
    } status;
    Position location;
    enum class Face : uint8_t {
        Bottom = 0,
        Top = 1,
        North = 2,
        South = 3,
        West = 4,
        East = 5,
    } face;
    int32_t sequence;
};
std::shared_ptr<PlayerAction> parsePlayerAction(std::vector<uint8_t> &buffer);

struct PlayerCommand : BaseServerPacket {
    int32_t entity_id;
    enum class ActionId : int32_t {
        StartSneaking = 0,
        StopSneaking = 1,
        LeaveBed = 2,
        StartSprinting = 3,
        StopSprinting = 4,
        StartJumpWithHorse = 5,
        StopJumpWithHorse = 6,
        OpenHorseInventory = 7,
        StartFlyingWithElytra = 8,
    } action_id;
    int32_t jump_boost;
};
std::shared_ptr<PlayerCommand> parsePlayerCommand(std::vector<uint8_t> &buffer);

struct PlayerInput : BaseServerPacket {
    float sideways;
    float forward;
    uint8_t flags;
};
std::shared_ptr<PlayerInput> parsePlayerInput(std::vector<uint8_t> &buffer);

struct Pong : BaseServerPacket {
    int32_t id;
};
std::shared_ptr<Pong> parsePong(std::vector<uint8_t> &buffer);

struct PlayerSession : BaseServerPacket {
    u128 uuid;
    long expires_at;
    std::vector<uint8_t> public_key;
    std::vector<uint8_t> signature;
};
std::shared_ptr<PlayerSession> parsePlayerSession(std::vector<uint8_t> &buffer);

struct ChangeRecipeBookSettings : BaseServerPacket {
    enum class BookID : int32_t {
        Crafting = 0,
        Furnace = 1,
        BlastFurnace = 2,
        Smoker = 3,
    } book_id;
    bool book_open;
    bool filter_active;
};
std::shared_ptr<ChangeRecipeBookSettings> parseChangeRecipeBookSettings(std::vector<uint8_t> &buffer);

struct SetSeenRecipe : BaseServerPacket {
    std::string recipe_id;
};
std::shared_ptr<SetSeenRecipe> parseSetSeenRecipe(std::vector<uint8_t> &buffer);

struct RenameItem : BaseServerPacket {
    std::string item_name;
};
std::shared_ptr<RenameItem> parseRenameItem(std::vector<uint8_t> &buffer);

struct ResourcePack : BaseServerPacket {
    enum class Result : int32_t {
        SuccessfullyLoaded = 0,
        Declined = 1,
        FailedDownload = 2,
        Accepted = 3,
    } result;
};
std::shared_ptr<ResourcePack> parseResourcePack(std::vector<uint8_t> &buffer);

struct SeenAdvancements : BaseServerPacket {
    enum class Action : int32_t {
        OpenedTab = 0,
        ClosedScreen = 1,
    } action;
    std::string tab_id;
};
std::shared_ptr<SeenAdvancements> parseSeenAdvancements(std::vector<uint8_t> &buffer);

struct SelectTrade : BaseServerPacket {
    int32_t selected_slot;
};
std::shared_ptr<SelectTrade> parseSelectTrade(std::vector<uint8_t> &buffer);

struct SetBeaconEffect : BaseServerPacket {
    bool primary_effect_present;
    int32_t primary_effect;
    bool secondary_effect_present;
    int32_t secondary_effect;
};
std::shared_ptr<SetBeaconEffect> parseSetBeaconEffect(std::vector<uint8_t> &buffer);

struct SetHeldItem : BaseServerPacket {
    uint16_t slot; // Why that a short Mojang ? A byte would have been way enough -_-
};
std::shared_ptr<SetHeldItem> parseSetHeldItem(std::vector<uint8_t> &buffer);

struct ProgramCommandBlock : BaseServerPacket {
    Position location;
    std::string command;
    enum class Mode : int32_t {
        Sequence = 0,
        Auto = 1,
        Redstone = 2,
    } mode;
    uint8_t flags;
};
std::shared_ptr<ProgramCommandBlock> parseProgramCommandBlock(std::vector<uint8_t> &buffer);

struct ProgramCommandBlockMinecart : BaseServerPacket {
    int32_t entity_id;
    std::string command;
    bool track_output;
};
std::shared_ptr<ProgramCommandBlockMinecart> parseProgramCommandBlockMinecart(std::vector<uint8_t> &buffer);

struct SetCreativeModeSlot : BaseServerPacket {
    int16_t slot;
    Slot clicked_item;
};
std::shared_ptr<SetCreativeModeSlot> parseSetCreativeModeSlot(std::vector<uint8_t> &buffer);

struct ProgramJigsawBlock : BaseServerPacket {
    Position location;
    std::string name;
    std::string target;
    std::string pool;
    std::string final_state;
    std::string joint_type;
};
std::shared_ptr<ProgramJigsawBlock> parseProgramJigsawBlock(std::vector<uint8_t> &buffer);

struct ProgramStructureBlock : BaseServerPacket {
    Position location;
    enum class Action : int32_t {
        UpdateData = 0,
        SaveTheStructure = 1,
        LoadTheStructure = 2,
        DetectSize = 3,
    } action;
    enum class Mode : int32_t {
        Save = 0,
        Load = 1,
        Corner = 2,
        Data = 3,
    } mode;
    std::string name;
    uint8_t offset_x;
    uint8_t offset_y;
    uint8_t offset_z;
    uint8_t size_x;
    uint8_t size_y;
    uint8_t size_z;
    enum class Mirror : int32_t {
        None = 0,
        LeftRight = 1,
        FrontBack = 2,
    } mirror;
    enum class Rotation : int32_t {
        None = 0,
        ClockWiseNinety = 1,
        ClockWiseOneEighty = 2,
        CounterClockWiseNinety = 3,
    } rotation;
    std::string metadata;
    float integrity;
    int64_t seed;
    uint8_t flags;
};
std::shared_ptr<ProgramStructureBlock> parseProgramStructureBlock(std::vector<uint8_t> &buffer);

struct UpdateSign : BaseServerPacket {
    Position location;
    std::string line_1;
    std::string line_2;
    std::string line_3;
    std::string line_4;
};
std::shared_ptr<UpdateSign> parseUpdateSign(std::vector<uint8_t> &buffer);

struct SwingArm : BaseServerPacket {
    enum class Hand : int32_t {
        MainHand = 0,
        OffHand = 1,
    } hand;
};
std::shared_ptr<SwingArm> parseSwingArm(std::vector<uint8_t> &buffer);

struct TeleportToEntity : BaseServerPacket {
    u128 target_player;
};
std::shared_ptr<TeleportToEntity> parseTeleportToEntity(std::vector<uint8_t> &buffer);

struct UseItemOn : BaseServerPacket {
    enum class Hand : int32_t {
        MainHand = 0,
        OffHand = 1,
    } hand;
    Position location;
    enum class Face : int32_t {
        Bottom = 0,
        Top = 1,
        North = 2,
        South = 3,
        West = 4,
        East = 5,
    } face;
    float cursor_position_x;
    float cursor_position_y;
    float cursor_position_z;
    bool inside_block;
    int32_t sequence;
};
std::shared_ptr<UseItemOn> parseUseItemOn(std::vector<uint8_t> &buffer);

struct UseItem : BaseServerPacket {
    enum class Hand : int32_t {
        MainHand = 0,
        OffHand = 1,
    } hand;
    int32_t sequence;
};
std::shared_ptr<UseItem> parseUseItem(std::vector<uint8_t> &buffer);

// Maps

static const std::unordered_map<ServerPacketsID, std::function<std::shared_ptr<BaseServerPacket>(std::vector<uint8_t> &)>> packetIDToParseInitial = {
    {ServerPacketsID::Handshake, parseHandshake},
};

static const std::unordered_map<ServerPacketsID, std::function<std::shared_ptr<BaseServerPacket>(std::vector<uint8_t> &)>> packetIDToParseStatus = {
    {ServerPacketsID::StatusRequest, &parseStatusRequest},
    {ServerPacketsID::PingRequest, &parsePingRequest},
};

static const std::unordered_map<ServerPacketsID, std::function<std::shared_ptr<BaseServerPacket>(std::vector<uint8_t> &)>> packetIDToParseLogin = {
    {ServerPacketsID::LoginStart, &parseLoginStart},
    {ServerPacketsID::EncryptionResponse, &parseEncryptionResponse},
};

static const std::unordered_map<ServerPacketsID, std::function<std::shared_ptr<BaseServerPacket>(std::vector<uint8_t> &)>> packetIDToParsePlay = {
    {ServerPacketsID::ConfirmTeleportation, &parseConfirmTeleportation},
    {ServerPacketsID::QueryBlockEntityTag, &parseQueryBlockEntityTag},
    {ServerPacketsID::ChangeDifficulty, &parseChangeDifficulty},
    {ServerPacketsID::ChatCommand, &parseChatCommand},
    {ServerPacketsID::ChatMessage, &parseChatMessage},
    {ServerPacketsID::ClientCommand, &parseClientCommand},
    {ServerPacketsID::ClientInformation, &parseClientInformation},
    {ServerPacketsID::CommandSuggestionRequest, &parseCommandSuggestionRequest},
    {ServerPacketsID::ClickContainerButton, &parseClickContainerButton},
    {ServerPacketsID::ClickContainer, &parseClickContainer},
    {ServerPacketsID::CloseContainerRequest, &parseCloseContainerRequest},
    {ServerPacketsID::PluginMessage, &parsePluginMessage},
    {ServerPacketsID::EditBook, &parseEditBook},
    {ServerPacketsID::QueryEntityTag, &parseQueryEntityTag},
    {ServerPacketsID::Interact, &parseInteract},
    {ServerPacketsID::JigsawGenerate, &parseJigsawGenerate},
    {ServerPacketsID::KeepAliveResponse, &parseKeepAliveResponse},
    {ServerPacketsID::LockDifficulty, &parseLockDifficulty},
    {ServerPacketsID::SetPlayerPosition, &parseSetPlayerPosition},
    {ServerPacketsID::SetPlayerPositionAndRotation, &parseSetPlayerPositionAndRotation},
    {ServerPacketsID::SetPlayerRotation, &parseSetPlayerRotation},
    {ServerPacketsID::SetPlayerOnGround, &parseSetPlayerOnGround},
    {ServerPacketsID::MoveVehicle, &parseMoveVehicle},
    {ServerPacketsID::PaddleBoat, &parsePaddleBoat},
    {ServerPacketsID::PickItem, &parsePickItem},
    {ServerPacketsID::PlaceRecipe, &parsePlaceRecipe},
    {ServerPacketsID::PlayerAbilities, &parsePlayerAbilities},
    {ServerPacketsID::PlayerAction, &parsePlayerAction},
    {ServerPacketsID::PlayerCommand, &parsePlayerCommand},
    {ServerPacketsID::PlayerInput, &parsePlayerInput},
    {ServerPacketsID::Pong, &parsePong},
    {ServerPacketsID::PlayerSession, &parsePlayerSession},
    {ServerPacketsID::ChangeRecipeBookSettings, &parseChangeRecipeBookSettings},
    {ServerPacketsID::SetSeenRecipe, &parseSetSeenRecipe},
    {ServerPacketsID::RenameItem, &parseRenameItem},
    {ServerPacketsID::ResourcePack, &parseResourcePack},
    {ServerPacketsID::SeenAdvancements, &parseSeenAdvancements},
    {ServerPacketsID::SelectTrade, &parseSelectTrade},
    {ServerPacketsID::SetBeaconEffect, &parseSetBeaconEffect},
    {ServerPacketsID::SetHeldItem, &parseSetHeldItem},
    {ServerPacketsID::ProgramCommandBlock, &parseProgramCommandBlock},
    {ServerPacketsID::ProgramCommandBlockMinecart, &parseProgramCommandBlockMinecart},
    {ServerPacketsID::SetCreativeModeSlot, &parseSetCreativeModeSlot},
    {ServerPacketsID::ProgramJigsawBlock, &parseProgramJigsawBlock},
    {ServerPacketsID::ProgramStructureBlock, &parseProgramStructureBlock},
    {ServerPacketsID::UpdateSign, &parseUpdateSign},
    {ServerPacketsID::SwingArm, &parseSwingArm},
    {ServerPacketsID::TeleportToEntity, &parseTeleportToEntity},
    {ServerPacketsID::UseItemOn, &parseUseItemOn},
    {ServerPacketsID::UseItem, &parseUseItem},
};
}

#endif /* AE9C1FA0_D3A2_4B7D_962E_4EAF72963603 */
