//
//  BigoAdInnerError.h
//  Pods
//
//  Created by 蔡庆敏 on 2021/6/1.
//

#ifndef BigoAdInnerErrorCode_h

//region Error code - 从 3000 开始
static NSInteger const BIGO_AD_ERROR_CODE_DEFAULT = 3000; // 默认
static NSInteger const BIGO_AD_ERROR_CODE_BANNER_RES = 3001; // Banner 资源相关
static NSInteger const BIGO_AD_ERROR_CODE_WEBVIEW = 3002; // WebView 相关
static NSInteger const BIGO_AD_ERROR_CODE_VIDEO_PLAYER = 3003; // 视频播放器相关
//endregion


//特定
static NSInteger const BIGO_AD_ERROR_CODE_UNKNOWN = 0;
static NSInteger const BIGO_AD_ERROR_CODE_NO_AD = -6;
static NSInteger const BIGO_AD_ERROR_CODE_APP_ID_UNMATCHED_ERROR = -9; // App id 不匹配，服务端规定

//其他
static NSInteger const BIGO_AD_ERROR_SUB_CODE_EMPTY_APP_KEY = 10000;
static NSInteger const BIGO_AD_ERROR_SUB_CODE_EMPTY_SLOT_ID = 10001;
static NSInteger const BIGO_AD_ERROR_SUB_CODE_CONFIG_PARSE_ERROR = 10002;
static NSInteger const BIGO_AD_ERROR_SUB_CODE_EMPTY_AD_SIZES = 10003;

static NSInteger const BIGO_AD_ERROR_SUB_CODE_CRASH_EXCEPTION = 10100;
static NSInteger const BIGO_AD_ERROR_SUB_CODE_DB_INIT_FAILED = 10101;
static NSInteger const BIGO_AD_ERROR_SUB_CODE_HTML_LOAD_ERROR = 10102;
static NSInteger const BIGO_AD_ERROR_SUB_CODE_DOWNLOAD_EMPTY_URL = 10103;
static NSInteger const BIGO_AD_ERROR_SUB_CODE_MISSING_HTML = 10104;
static NSInteger const BIGO_AD_ERROR_SUB_CODE_WEBVIEW_RENDER_PROCESS_GONE = 10105;
static NSInteger const BIGO_AD_ERROR_SUB_CODE_WEBVIEW_TRACKER_ERROR = 10106;
static NSInteger const BIGO_AD_ERROR_SUB_CODE_VIDEO_NOT_PREPARED = 10107;


//vast error
static NSInteger const BGAD_ERROR_CODE_VAST_PARSE_ERROR = 1006;

static NSInteger const BGAD_ERROR_SUB_CODE_DELIVERY_PARAM_INVALIDATE = 10060;
static NSInteger const BGAD_ERROR_SUB_CODE_VAST_XML_PARAM_EXCEPTION = 10061;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_NOT_AD_NODE = 10062;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_NO_MATCH_MEDIA_OTHRER = 10063;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_NOT_WRAPPER_NODE = 10064;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_NO_MATCH_MEDIA_NODE_EMPTY = 10065;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_NO_MATCH_MEDIA_MIMETYPE_NOSUPPORT = 10066;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_NO_MATCH_MEDIA_URL_EMPTY = 10067;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_NO_MATCH_MEDIA_WIDTH_TO_HEIGHT_RATIO_FOR_VIDEO_DIRECTION = 10068;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_NO_MATCH_MEDIA_BITRATE_NOMATCH = 10069;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_REDIRECT_URL_INVALIDATE = 10070;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_REDIRECT_OVER_LIMIT = 10071;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_REQUEST_FAILED = 10072;
static NSInteger const BGAD_ERROR_SUB_CODE_WRAPPER_RESPONSE_PROCESS_ERROR = 10073;


static NSInteger const BGAD_ERROR_SUB_CODE_SERVER_BIDDING_NO_PAYLOAD = 10200;  // Server bidding类型的广告没有payload
static NSInteger const BGAD_ERROR_SUB_CODE_SERVER_BIDDING_PAYLOAD_ERROR = 10201;  // Server bidding类型的广告payload无效

static NSInteger const BGAD_ERROR_SUB_CODE_MISSING_USER_CONSENT = 10300; //缺少用户同意权

#endif /* BigoAdInnerErrorCode_h */