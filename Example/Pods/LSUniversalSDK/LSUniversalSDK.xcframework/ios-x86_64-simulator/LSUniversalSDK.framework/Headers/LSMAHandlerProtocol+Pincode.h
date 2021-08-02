//
//  LSMAHandlerProtocol+Pincode.h
//  LSUniversalSDK
//

#import <LSUniversalSDK/LSMAHandlerProtocol+Register.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * An invitation description is created when an agent sends a pincode. This invitation keeps all information
 * given and received from the back end.
 */
@interface LSMAInvitationDescription: NSObject <NSCoding>

/**
 * Either the phone number or email address used to send the invitation. If self.media == URL, this is empty.
 */
@property (nonatomic, readonly) NSString *recipient;

/**
 * The reference linked to the call. Either filled by the agent, or added by the backend.
 */
@property (nonatomic, readonly, nullable) NSString *reference;

/**
 * Usecase used to start the call
 */
@property (nonatomic, readonly, nonnull) NSObject <LSMAUsecase> *usecase;

/**
 * Media used to generate the pincode.
 */
@property (nonatomic, readonly) LSMAUsecaseMedia_t media;

@property (nonatomic, readonly) LSMAUsecaseMode_t mode;

/**
 * The pincode generated by the backend.
 */
@property (nonatomic, readonly, nullable) NSString *pincodeID;

/**
 * The invite URL generated by the backend.
 */
@property (nonatomic, readonly, nullable) NSString *inviteURL;

/**
 * Indicates whether the pincode was resent
 */
@property (nonatomic, readonly) BOOL resent;

@end


/**
 * The pincodeDelegate protocol.
 */
@protocol LSMAPincodeEvent

/**
 * A pincode available for this account was created/resent/cancelled by another mean that this SDK.
 */
- (void)pincodeEvent:(LSMAPincodeStatus_t)event;

@end

/**
 * This interface implemented by the `LSUniversal.agent` property deals with pincodes.
 */
@protocol LSMAHandlerPincode <LSMAHandler>

/**
 * The pincode delegate is notified when a pincode is created/updated with the user's account.
 */
@property (nonatomic, weak, nullable) NSObject <LSMAPincodeEvent> *pincodeDelegate;

/**
 * The latestInvitation is kept across app reboot.
 */
@property (nonatomic, readonly, nullable) LSMAInvitationDescription *latestInvitation;

/**
 *  Sends a pincode to a Guest. This pincode will be sent as an email.
 *  The latestInvitation is replaced by a new invitationDescription containing all info about this pincode.
 *  @discussion To accept this invitation, the Guest needs to start the LSUniversal with the URL received.
 *
 *  @param usecase      The usecase
 *  @param phoneNumber  The phone number. her in international format (ex +33601234567) or national format and we'll try to guess the country code.
 *  @param reference    The reference for the pincode
 *  @param block        The notification block. This block contains the pincode sent to the recipient.
 */
- (void)sendInvitationForUsecase:(NSObject<LSMAUsecase> *_Nonnull)usecase
                           toPhone:(nonnull NSString *)phoneNumber
                     withReference:(nullable NSString *)reference
                         andNotify:(void(^_Nullable)(LSMAPincodeStatus_t status, NSString *_Nullable pincode))block;

/**
 *  Sends pincode to a Guest. This pincode will be sent as an email.
 *  The latestInvitation is replaced by a new invitationDescription containing all info about this pincode.
 *  @discussion To accept this invitation, the Guest needs to start the LSUniversal with the URL received.
 *
 *  @param usecase      The usecase
 *  @param email        The email to send the notification to
 *  @param reference    The reference for the usecase
 *  @param block        The notification block. This block contains the pincode sent to the recipient.
 */
- (void)sendInvitationForUsecase:(NSObject<LSMAUsecase> *_Nonnull)usecase
                           toEmail:(nonnull NSString *)email
                     withReference:(nullable NSString *)reference
                         andNotify:(void(^_Nullable)(LSMAPincodeStatus_t status, NSString *_Nullable pincode))block;

/**
 *  Requests a pincode from the backend without sending an email or sms. This returns the URL that would have been sent to the guest.
 *  The latestInvitation is replaced by a new invitationDescription containing all info about this pincode.
 *  @param usecase      The usecase object containing the information to be used
 *  @param reference    The display name of the invitation
 *  @param block        The block to be executed after the URL is received or in case of error.
 */
- (void)createInvitationForUsecase:(NSObject<LSMAUsecase> *_Nonnull)usecase
                 withReference:(nullable NSString *)reference
                     andNotify:(void(^_Nullable)(LSMAPincodeStatus_t status, NSString *_Nullable inviteURL))block;

/**
 * Cancels an invitation based on the suffix. An invitation created through the SDK should be cancelled by the SDK.
 * @param block         Code block exectuted once the pincode is cancelled.
 */
- (void)cancelPincodeAndExecute:(void(^_Nullable)(void))block;

/**
 * Asks for the pincode to be resent to its recipient.
 * @param block         Code block executed once the call to resent the pincode is called.
 */
- (void)resendPincodeAndExecute:(void(^_Nullable)(LSMAPincodeStatus_t status))block;

/**
 * Lists the last invitations sent.
 */
@property (nonatomic, readonly, nullable) NSArray <LSMAInvitationDescription *> *invitationHistory;

@end

NS_ASSUME_NONNULL_END
