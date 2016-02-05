/*
 * File:   RTPPackage.h
 * Author: daniel
 *
 * Created on March 28, 2015, 12:30 PM
 */

#ifndef RTPPACKAGEHANDLER_H
#define	RTPPACKAGEHANDLER_H

#include <random> // random generator objects
#include <chrono> // clock, tick
#include <string.h> //memcpy


//For htons/htonl and ntohs/ntohl
#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

/*!
 * Minimum size of a RTP-Header in bytes, without any CSRCs set
 */
static const unsigned int RTP_HEADER_MIN_SIZE = 12;

/*!
 * Maximum size of a RTP-Header with all CSRCs set.
 */
static const unsigned int RTP_HEADER_MAX_SIZE = 72;



/*!
 * List of default mappings for payload-type, as specified in https://www.ietf.org/rfc/rfc3551.txt
 *
 * Also see: https://en.wikipedia.org/wiki/RTP_audio_video_profile
 *
 * Currently only containing audio mappings.
 */
enum PayloadType
{
    //ITU-T G.711 PCMU - https://en.wikipedia.org/wiki/PCMU
    PCMU = 0,
    //GSM Full Rate - https://en.wikipedia.org/wiki/Full_Rate
    GSM = 3,
    //ITU-T G.723.1 - https://en.wikipedia.org/wiki/G.723.1
    G723 = 4,
    //IMA ADPCM 32 kbit/s - https://en.wikipedia.org/wiki/Adaptive_differential_pulse-code_modulation
    DVI4_32 = 5,
    //IMA ADPCM 64 kbit/s
    DVI4_64 = 6,
    //LPC - https://en.wikipedia.org/wiki/Linear_predictive_coding
    LPC = 7,
    //ITU-T G.711 PCMA - https://en.wikipedia.org/wiki/PCMA
    PCMA = 8,
    //ITU-T G.722 - https://en.wikipedia.org/wiki/G.722
    G722 = 9,
    //Linear PCM, 2 channels - https://en.wikipedia.org/wiki/Linear_PCM
    L16_2 = 10,
    //Linear PCM, 1 channel - https://en.wikipedia.org/wiki/Linear_PCM
    L16_1 = 11,
    //Qualcomm Code Excited Linear Prediction
    QCELP = 12,
    //Comfort noise
    CN = 13,
    //MPEG-1 or MPEG-2 audio - https://en.wikipedia.org/wiki/MPEG-1 / https://en.wikipedia.org/wiki/MPEG-2
    MPA = 14,
    //ITU-T G.728
    G728 = 15,
    //IMA ADPCM 44.1 kbit/s
    DVI4_44 = 16,
    //IMA ADPCM 88.2 kbit/s
    DVI4_88 = 17,
    //ITU-T G.729(a) - https://en.wikipedia.org/wiki/G.729
    G729 = 18,
    //OPUS variable bandwidth - https://en.wikipedia.org/wiki/Opus_%28audio_format%29
    //RFC 7587 (RTP Payload Format for Opus, see: https://ietf.org/rfc/rfc7587.txt) defines the opus payload-type as dynamic
    OPUS = 112,
    //dummy payload-type to accept all types
    ALL = -1

};

/*!
 *
 * The RTP header is specified in RFC 3550 has the following format:
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |V=2|P|X|  CC   |M|     PT      |       sequence number         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                           timestamp                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |           synchronization source (SSRC) identifier            |
 * +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 * |            contributing source (CSRC) identifiers             |
 * |                             ....                              |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * The first twelve octets are present in every RTP packet, while the
 * list of CSRC identifiers is present only when inserted by a mixer.
 * The fields have the following meaning:
 *
 * version (V): 2 bits
 *  This field identifies the version of RTP. The version defined by this specification is two (2).
 *
 * padding (P): 1 bit
 *  If the padding bit is set, the packet contains one or more additional padding octets at the end
 *  which are not part of the payload. The last octet of the padding contains a count of how many padding octets
 *  should be ignored, including itself. Padding may be needed by some encryption algorithms with fixed block sizes
 *  or for carrying several RTP packets in a lower-layer protocol data unit.
 *
 * extension (X): 1 bit
 *  If the extension bit is set, the fixed header MUST be followed by exactly one header extension,
 *  with a format defined in Section 5.3.1.
 *
 * CSRC count (CC): 4 bits
 *  The CSRC count contains the number of CSRC identifiers that follow the fixed header.
 *
 * marker (M): 1 bit
 *  The interpretation of the marker is defined by a profile. It is intended to allow significant events
 *  such as frame boundaries to be marked in the packet stream. A profile MAY define additional marker bits
 *  or specify that there is no marker bit by changing the number of bits in the payload type field (see Section 5.3).
 *
 * payload type (PT): 7 bits
 *  This field identifies the format of the RTP payload and determines its interpretation by the application.
 *  A profile MAY specify a default static mapping of payload type codes to payload formats.
 *  Additional payload type codes MAY be defined dynamically through non-RTP means (see Section 3).
 *  A set of default mappings for audio and video is specified in the companion RFC 3551 [1].
 *  An RTP source MAY change the payload type during a session, but this field SHOULD NOT be used
 *  for multiplexing separate media streams (see Section 5.2).
 *  A receiver MUST ignore packets with payload types that it does not understand.
 *
 * sequence number: 16 bits
 *  The sequence number increments by one for each RTP data packet sent, and may be used by the receiver
 *  to detect packet loss and to restore packet sequence. The initial value of the sequence number SHOULD be random
 *  (unpredictable) to make known-plaintext attacks on encryption more difficult, even if the source itself does not
 *  encrypt according to the method in Section 9.1, because the packets may flow through a translator that does.
 *
 * timestamp: 32 bits
 *  The timestamp reflects the sampling instant of the first octet in the RTP data packet.
 *  The sampling instant MUST be derived from a clock that increments monotonically and linearly in time to allow
 *  synchronization and jitter calculations (see Section 6.4.1). The resolution of the clock MUST be sufficient
 *  for the desired synchronization accuracy and for measuring packet arrival jitter
 *  (one tick per video frame is typically not sufficient). The clock frequency is dependent on the format of data
 *  carried as payload and is specified statically in the profile or payload format specification
 *  that defines the format, or MAY be specified dynamically for payload formats defined through non-RTP means.
 *  If RTP packets are generated periodically, the nominal sampling instant as determined from the sampling clock
 *  is to be used, not a reading of the system clock.
 *  As an example, for fixed-rate audio the timestamp clock would likely increment by one for each sampling period.
 *  If an audio application reads blocks covering 160 sampling periods from the input device,
 *  the timestamp would be increased by 160 for each such block, regardless of whether the block is transmitted
 *  in a packet or dropped as silent.
 *  The initial value of the timestamp SHOULD be random, as for the sequence number.
 *  Several consecutive RTP packets will have equal timestamps if they are (logically) generated at once,
 *  e.g., belong to the same video frame.  Consecutive RTP packets MAY contain timestamps that are not monotonic
 *  if the data is not transmitted in the order it was sampled, as in the case of MPEG interpolated video frames.
 *  RTP timestamps from different media streams may advance at different rates and usually have independent,
 *  random offsets.
 *  The sampling instant is chosen as the point of reference for the RTP timestamp because it is known
 *  to the transmitting endpoint and has a common definition for all media,
 *  independent of encoding delays or other processing.
 *  Applications transmitting stored data rather than data sampled in real time typically use a virtual presentation
 *  timeline derived from wallclock time to determine when the next frame or other unit of each medium
 *  in the stored data should be presented.
 *  NOTE: in our implementation, the RT(C)P timestamp is always in milliseconds precision
 *
 * SSRC: 32 bits
 *  The SSRC field identifies the synchronization source. This identifier SHOULD be chosen randomly,
 *  with the intent that no two synchronization sources within the same RTP session will have the same SSRC identifier.
 *  An example algorithm for generating a random identifier is presented in Appendix A.6.
 *  Although the probability of multiple sources choosing the same identifier is low,
 *  all RTP implementations must be prepared to detect and resolve collisions. Section 8 describes the probability of
 *  collision along with a mechanism for resolving collisions and detecting RTP-level forwarding loops
 *  based on the uniqueness of the SSRC identifier. If a source changes its source transport address,
 *  it must also choose a new SSRC identifier to avoid being interpreted as a looped source (see Section 8.2).
 *
 * CSRC list: 0 to 15 items, 32 bits each
 *  The CSRC list identifies the contributing sources for the payload contained in this packet.
 *  The number of identifiers is given by the CC field. If there are more than 15 contributing sources,
 *  only 15 can be identified. CSRC identifiers are inserted by mixers (see Section 7.1), using the SSRC identifiers of
 *  contributing sources. For example, for audio packets the SSRC identifiers of all sources that were mixed together
 *  to create a packet are listed, allowing correct talker indication at the receiver.
 */
struct RTPHeader
{
private:
    
    //data[0]
    //2 bit version field
    //1 bit padding flag
    //1 bit extension flag
    //4 bit CSRC count field
    
    //data[1]
    //1 bit marker flag
    //7 bit payload type field
    
    //data[2-3]
    //16 bit sequence number field
    
    //data[4-7]
    //32 bit timestamp field
    
    //data[8-11]
    //32 bit SSRC field
    uint8_t data[12];

    //list of 32 bit CSRCs
    //TODO adding this requires correct handling of RTPHeader (depending on the csrc_count)
    //uint32_t csrc_list[15];
    
    static const unsigned int shiftVersion = 6;
    static const unsigned int shiftPadding = 5;
    static const unsigned int shiftExtension = 4;
    static const unsigned int shiftMarker = 7;
    
public:
    
    /*!
     * The version-flag of a RTP package is always 2
     */
    static const unsigned int VERSION = 2;
    
    RTPHeader() : data{0}
    {
        data[0] = VERSION << shiftVersion;
    }
    
    inline uint8_t getVersion() const
    {
        return (data[0] >> shiftVersion) & 0x3;
    }
    
    inline bool isPadded() const
    {
        return (data[0] >> shiftPadding) & 0x1;
    }
    
    inline void setPadding(bool padded)
    {
        data[0] = data[0] | (padded << shiftPadding);
    }
    
    inline bool hasExtension() const
    {
        return (data[0] >> shiftExtension) & 0x1;
    }
    
    inline void setExtension(bool extension)
    {
        data[0] = data[0] | (extension << shiftExtension);
    }
    
    inline uint8_t getCSRCCount() const
    {
        return data[0] & 0x7;
    }
    
    inline void setCSRCCount(uint8_t csrcCount)
    {
        data[0] = data[0] | (csrcCount & 0x7);
    }
    
    inline bool isMarked() const
    {
        return (data[1] >> shiftMarker) & 0x1;
    }
    
    inline void setMarker(bool marker)
    {
        data[1] = data[1] | (marker << shiftMarker);
    }
    
    inline PayloadType getPayloadType() const
    {
        return (PayloadType)(data[1] & 0x7F);
    }
    
    inline void setPayloadType(PayloadType type)
    {
        data[1] = data[1] | (type & 0x7F);
    }
        
    inline uint16_t getSequenceNumber() const
    {
        return ntohs((data[3] << 8) | data[2]);
    }
    
    inline void setSequenceNumber(const uint16_t sequenceNumber)
    {
        data[3] = (uint8_t) (htons(sequenceNumber) >> 8);
        data[2] = (uint8_t) (htons(sequenceNumber) & 0xFF);
    }
    
    inline uint32_t getTimestamp() const
    {
        return ntohl(data[7] << 24 | data[6] << 16 | data[5] << 8 | data[4]);
    }
    
    inline void setTimestamp(const uint32_t timstamp)
    {
        uint32_t tmp = htonl(timstamp);
        data[7] = (uint8_t) (tmp >> 24);
        data[6] = (uint8_t) (tmp >> 16);
        data[5] = (uint8_t) (tmp >> 8);
        data[4] = (uint8_t) (tmp & 0xFF);
    }
    
    inline uint32_t getSSRC() const
    {
        return ntohl(data[11] << 24 | data[10] << 16 | data[9] << 8 | data[8]);
    }
    
    inline void setSSRC(const uint32_t ssrc)
    {
        uint32_t tmp = htonl(ssrc);
        data[11] = (uint8_t) (tmp >> 24);
        data[10] = (uint8_t) (tmp >> 16);
        data[9] = (uint8_t) (tmp >> 8);
        data[8] = (uint8_t) (tmp & 0xFF);
    }
};

class RTPPackageHandler
{
public:
    /*!
     * Constructs a new RTPPackageHandler-object
     *
     * \param maximumPayloadSize The maximum size in bytes of the payload (body)
     *
     * \param payloadType The PayloadType, defaults to L16_2
     *
     */
    RTPPackageHandler(unsigned int maximumPayloadSize, PayloadType payloadType = L16_2);

    ~RTPPackageHandler();
    
    /*!
     * Generates a new RTP-package by generating the header and copying the payload-data (audio data)
     *
     * \param audioData The payload for the new RTP-package
     *
     * \param payloadSize The size in bytes of the audio-payload,
     *  must be smaller or equals to #getMaximumPayloadSize()
     *
     * Returns a pointer to the internal buffer storing the new package
     */
    const void* createNewRTPPackage(const void* audioData, unsigned int payloadSize);

    /*!
     * Returns a pointer to the payload of the internal RTP-package
     */
    const void* getRTPPackageData() const;

    /*!
     * Returns a RTPHeader pointer of the currently stored RTP-package.
     */
    const RTPHeader* getRTPPackageHeader() const;
    
    /*!
     * Returns a RTPHeaderExtension pointer to the extension in the stored RTP-package. 
     * If no such extension exists, nullptr is returned.
     */
    const RTPHeaderExtension getRTPHeaderExtension() const;

    /*!
     * Gets the maximum size for the RTP package (header + body)
     */
    unsigned int getMaximumPackageSize() const;

    /*!
     * Gets the RTPHeader size
     */
    unsigned int getRTPHeaderSize() const;
    
    /*!
     * Returns the size (in bytes) of the currently stored RTP header-extension
     */
    unsigned int getRTPHeaderExtensionSize() const;

    /*!
     * Gets the maximum payload size
     */
    unsigned int getMaximumPayloadSize() const;

    /*!
     * Returns the actual size of the currently buffered audio-payload
     *
     * Note: This value is only accurate if the #setActualPayloadSize() was set for the current payload
     */
    unsigned int getActualPayloadSize() const;

    /*!
     * Sets the payload-size in bytes of the currently buffered package
     *
     * \param payloadSize The new size in bytes
     */
    void setActualPayloadSize(unsigned int payloadSize);

    /*!
     * Returns the internal buffer, which could be used as receive buffer
     */
    void* getWorkBuffer();

    /*!
     * Creates a silence-package in the internal work-buffer.
     *
     * A silence-package is a RTP-package with a dummy header and zeroed out payload resulting in silence on playback.
     */
    void createSilencePackage();

    /*!
     * Returns this device SSRC
     */
    uint32_t getSSRC() const;
    
    /*!
     * Returns the current RTP timestamp for the internal clock
     */
    uint32_t getCurrentRTPTimestamp() const;
    
    /*!
     * This method tries to determine whether the received buffer holds an RTP package.
     *
     * NOTE: There may be some type of packages, which are not distinguished and falsely accepted as valid RTP-package.
     *
     * \param packageBuffer The buffer storing the package-data
     *
     * \param packageLength The length of the package in bytes
     *
     * \return Whether this buffer COULD be holding hold an RTP package
     */
    static bool isRTPPackage(const void* packageBuffer, unsigned int packageLength );

private:
    // A buffer that can store a whole RTP-Package
    void *workBuffer;

    unsigned int getRandomNumber();
    unsigned int createStartingTimestamp();
    unsigned int getAudioSourceId();

    std::mt19937 randomGenerator;


    unsigned int sequenceNr;
    unsigned int timestamp;
    unsigned int ssrc;
    PayloadType payloadType;

    unsigned int currentBufferSize;
    unsigned int maximumPayloadSize;
    unsigned int maximumBufferSize;
    unsigned int actualPayloadSize;
    
    friend class ProcessorRTP;
};

#endif	/* RTPPACKAGEHANDLER_H */

