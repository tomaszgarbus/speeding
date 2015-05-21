/**
 * @file gpg/i_real_time_event_listener.h
 * @brief An interface for listening to <code>RealTimeRoom</code> events.
 */

#ifndef GPG_I_REAL_TIME_EVENT_LISTENER_H_
#define GPG_I_REAL_TIME_EVENT_LISTENER_H_

#include <vector>
#include "gpg/multiplayer_participant.h"
#include "gpg/real_time_room.h"

namespace gpg {

/**
 * Defines an interface which can be delivererd events relating to real-time
 * multiplayer.
 */
class GPG_EXPORT IRealTimeEventListener {
 public:
  virtual ~IRealTimeEventListener() {}

  /**
   * <code>OnRoomStatusChanged</code> is called when a
   * <code>RealTimeRoom</code>'s <code>Status()</code> is updated.
   *
   * @param room The room which changed status.
   */
  virtual void OnRoomStatusChanged(RealTimeRoom const &room) = 0;

  /**
   * <code>OnConnectedSetChanged</code> is called when a
   * <code>MultiplayerParticipant</code> connects or disconnects from the room's
   * connected set.
   *
   * @param room The room whose connected set changed.
   */
  virtual void OnConnectedSetChanged(RealTimeRoom const &room) = 0;

  /**
   * <code>OnP2PConnected</code> is called when a
   * <code>MultiplayerParticipant</code> connects directly to the local player.
   *
   * @param room The room which <code>participant</code> is in.
   * @param participant The participant which connected.
   */
  virtual void OnP2PConnected(RealTimeRoom const &room,
                              MultiplayerParticipant const &participant) = 0;

  /**
   * <code>OnP2PDisconnected</code> is called when a
   * <code>MultiplayerParticipant</code> disconnects directly to the local
   * player.
   *
   * @param room The room which <code>participant</code> is in.
   * @param participant The participant which disconnected.
   */
  virtual void OnP2PDisconnected(RealTimeRoom const &room,
                                 MultiplayerParticipant const &participant) = 0;

  /**
   * <code>OnParticipantStatusChanged</code> is called when a
   * <code>MultiplayerParticipant</code>'s <code>Status()</code> changes.
   *
   * @param room The room which <code>participant</code> is in.
   * @param participant The participant whose status changed.
   */
  virtual void OnParticipantStatusChanged(
      RealTimeRoom const &room, MultiplayerParticipant const &participant) = 0;

  /**
   * <code>OnDataReceived</code> is called whenever data is received from
   * another <code>MultiplayerParticipant</code>.
   *
   * @param room The room which <code>from_participant</code> is in.
   * @param from_participant The participant who sent the data.
   * @param data The data which was recieved.
   * @param is_reliable Whether the data was sent using the unreliable or
   *                    reliable mechanism.
   */
  virtual void OnDataReceived(RealTimeRoom const &room,
                              MultiplayerParticipant const &from_participant,
                              std::vector<uint8_t> data,
                              bool is_reliable) = 0;
};

}  // namespace gpg

#endif  // GPG_I_REAL_TIME_EVENT_LISTENER_H_
