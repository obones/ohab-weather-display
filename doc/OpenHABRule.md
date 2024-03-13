# OpenHAB rule

For OpenHAB to provide the proper value, a rule must be written, and Javascript is the most readily available.
This rule would need to run once per minute to provide values that are reasonably fresh to the display.

With OpenHAB 4.1 there are some missing elements in the provided standard libraries so we had to implement the missing elements via minified polyfills from the following sources:

1. TextEncoder/TextDecoder: https://github.com/anonyco/FastestSmallestTextEncoderDecoder
2. btoa: https://gist.github.com/enepomnyaschih/72c423f727d395eeaa09697058238727

The flatbuffers JS library is directly imported as found in the `flatbuffers.min.js` file

The flatc generated classes are directly pasted into the rule which gives the following final content:

```javascript
        (function(){

          // Polyfill for TextEncoder/TextDecoder
          (function(e){function r(b){var a=b.charCodeAt(0),c=1114112,d=0,n=b.length|0,g="";switch(a>>>4){case 12:case 13:c=(a&31)<<6|b.charCodeAt(1)&63;d=128>c?0:2;break;case 14:c=(a&15)<<12|(b.charCodeAt(1)&63)<<6|b.charCodeAt(2)&63;d=2048>c?0:3;break;case 15:30===a>>>3&&(c=(a&7)<<18|(b.charCodeAt(1)&63)<<12|(b.charCodeAt(2)&63)<<6|b.charCodeAt(3),d=65536>c?0:4)}d&&(n<d?d=0:65536>c?g=f(c):1114112>c?(c=c-65664|0,g=f((c>>>10)+55296|0,(c&1023)+56320|0)):d=0);for(;d<n;d=d+1|0)g+="\ufffd";return g}function p(){}function t(b){var a=b.charCodeAt(0)|0;if(55296<=a&&56319>=a)if(b=b.charCodeAt(1)|0,56320<=b&&57343>=b){if(a=(a<<10)+b-56613888|0,65535<a)return f(240|a>>>18,128|a>>>12&63,128|a>>>6&63,128|a&63)}else a=65533;return 2047>=a?f(192|a>>>6,128|a&63):f(224|a>>>12,128|a>>>6&63,128|a&63)}function q(){}var f=String.fromCharCode,l={}.toString,h=e.SharedArrayBuffer,u=h?l.call(h):"",k=e.Uint8Array,m=k||Array,v=l.call((k?ArrayBuffer:m).prototype);h=q.prototype;var w=e.TextEncoder;p.prototype.decode=function(b){var a=b&&b.buffer||b,c=l.call(a);if(c!==v&&c!==u&&void 0!==b)throw TypeError("Failed to execute 'decode' on 'TextDecoder': The provided value is not of type '(ArrayBuffer or ArrayBufferView)'");b=k?new m(a):a;a="";c=0;for(var d=b.length|0;c<d;c=c+32768|0)a+=f.apply(0,b[k?"subarray":"slice"](c,c+32768|0));return a.replace(/[\xc0-\xff][\x80-\xbf]+|[\x80-\xff]/g,r)};e.TextDecoder||(e.TextDecoder=p);h.encode=function(b){b=void 0===b?"":(""+b).replace(/[\x80-\uD7ff\uDC00-\uFFFF]|[\uD800-\uDBFF][\uDC00-\uDFFF]?/g,t);for(var a=b.length|0,c=new m(a),d=0;d<a;d=d+1|0)c[d]=b.charCodeAt(d);return c};w||(e.TextEncoder=q)})(""+void 0==typeof global?""+void 0==typeof self?this:self:global);//AnonyCo//# sourceMappingURL=https://cdn.jsdelivr.net/gh/AnonyCo/FastestSmallestTextEncoderDecoder/EncoderDecoderTogether.min.js.map

          // flatbuffers.min.js
          let flatbuffers=(()=>{var b=(l,t)=>()=>(t||l((t={exports:{}}).exports,t),t.exports);var _=b(c=>{"use strict";Object.defineProperty(c,"__esModule",{value:!0});c.SIZE_PREFIX_LENGTH=c.FILE_IDENTIFIER_LENGTH=c.SIZEOF_INT=c.SIZEOF_SHORT=void 0;c.SIZEOF_SHORT=2;c.SIZEOF_INT=4;c.FILE_IDENTIFIER_LENGTH=4;c.SIZE_PREFIX_LENGTH=4});var y=b(o=>{"use strict";Object.defineProperty(o,"__esModule",{value:!0});o.isLittleEndian=o.float64=o.float32=o.int32=void 0;o.int32=new Int32Array(2);o.float32=new Float32Array(o.int32.buffer);o.float64=new Float64Array(o.int32.buffer);o.isLittleEndian=new Uint16Array(new Uint8Array([1,0]).buffer)[0]===1});var g=b(f=>{"use strict";Object.defineProperty(f,"__esModule",{value:!0});f.Encoding=void 0;var S;(function(l){l[l.UTF8_BYTES=1]="UTF8_BYTES",l[l.UTF16_STRING=2]="UTF16_STRING"})(S||(f.Encoding=S={}))});var T=b(E=>{"use strict";Object.defineProperty(E,"__esModule",{value:!0});E.ByteBuffer=void 0;var d=_(),n=y(),m=g(),w=class l{constructor(t){this.bytes_=t,this.position_=0,this.text_decoder_=new TextDecoder}static allocate(t){return new l(new Uint8Array(t))}clear(){this.position_=0}bytes(){return this.bytes_}position(){return this.position_}setPosition(t){this.position_=t}capacity(){return this.bytes_.length}readInt8(t){return this.readUint8(t)<<24>>24}readUint8(t){return this.bytes_[t]}readInt16(t){return this.readUint16(t)<<16>>16}readUint16(t){return this.bytes_[t]|this.bytes_[t+1]<<8}readInt32(t){return this.bytes_[t]|this.bytes_[t+1]<<8|this.bytes_[t+2]<<16|this.bytes_[t+3]<<24}readUint32(t){return this.readInt32(t)>>>0}readInt64(t){return BigInt.asIntN(64,BigInt(this.readUint32(t))+(BigInt(this.readUint32(t+4))<<BigInt(32)))}readUint64(t){return BigInt.asUintN(64,BigInt(this.readUint32(t))+(BigInt(this.readUint32(t+4))<<BigInt(32)))}readFloat32(t){return n.int32[0]=this.readInt32(t),n.float32[0]}readFloat64(t){return n.int32[n.isLittleEndian?0:1]=this.readInt32(t),n.int32[n.isLittleEndian?1:0]=this.readInt32(t+4),n.float64[0]}writeInt8(t,e){this.bytes_[t]=e}writeUint8(t,e){this.bytes_[t]=e}writeInt16(t,e){this.bytes_[t]=e,this.bytes_[t+1]=e>>8}writeUint16(t,e){this.bytes_[t]=e,this.bytes_[t+1]=e>>8}writeInt32(t,e){this.bytes_[t]=e,this.bytes_[t+1]=e>>8,this.bytes_[t+2]=e>>16,this.bytes_[t+3]=e>>24}writeUint32(t,e){this.bytes_[t]=e,this.bytes_[t+1]=e>>8,this.bytes_[t+2]=e>>16,this.bytes_[t+3]=e>>24}writeInt64(t,e){this.writeInt32(t,Number(BigInt.asIntN(32,e))),this.writeInt32(t+4,Number(BigInt.asIntN(32,e>>BigInt(32))))}writeUint64(t,e){this.writeUint32(t,Number(BigInt.asUintN(32,e))),this.writeUint32(t+4,Number(BigInt.asUintN(32,e>>BigInt(32))))}writeFloat32(t,e){n.float32[0]=e,this.writeInt32(t,n.int32[0])}writeFloat64(t,e){n.float64[0]=e,this.writeInt32(t,n.int32[n.isLittleEndian?0:1]),this.writeInt32(t+4,n.int32[n.isLittleEndian?1:0])}getBufferIdentifier(){if(this.bytes_.length<this.position_+d.SIZEOF_INT+d.FILE_IDENTIFIER_LENGTH)throw new Error("FlatBuffers: ByteBuffer is too short to contain an identifier.");let t="";for(let e=0;e<d.FILE_IDENTIFIER_LENGTH;e++)t+=String.fromCharCode(this.readInt8(this.position_+d.SIZEOF_INT+e));return t}__offset(t,e){let i=t-this.readInt32(t);return e<this.readInt16(i)?this.readInt16(i+e):0}__union(t,e){return t.bb_pos=e+this.readInt32(e),t.bb=this,t}__string(t,e){t+=this.readInt32(t);let i=this.readInt32(t);t+=d.SIZEOF_INT;let s=this.bytes_.subarray(t,t+i);return e===m.Encoding.UTF8_BYTES?s:this.text_decoder_.decode(s)}__union_with_string(t,e){return typeof t=="string"?this.__string(e):this.__union(t,e)}__indirect(t){return t+this.readInt32(t)}__vector(t){return t+this.readInt32(t)+d.SIZEOF_INT}__vector_len(t){return this.readInt32(t+this.readInt32(t))}__has_identifier(t){if(t.length!=d.FILE_IDENTIFIER_LENGTH)throw new Error("FlatBuffers: file identifier must be length "+d.FILE_IDENTIFIER_LENGTH);for(let e=0;e<d.FILE_IDENTIFIER_LENGTH;e++)if(t.charCodeAt(e)!=this.readInt8(this.position()+d.SIZEOF_INT+e))return!1;return!0}createScalarList(t,e){let i=[];for(let s=0;s<e;++s){let a=t(s);a!==null&&i.push(a)}return i}createObjList(t,e){let i=[];for(let s=0;s<e;++s){let a=t(s);a!==null&&i.push(a.unpack())}return i}};E.ByteBuffer=w});var L=b(p=>{"use strict";Object.defineProperty(p,"__esModule",{value:!0});p.Builder=void 0;var B=T(),h=_(),N=class l{constructor(t){this.minalign=1,this.vtable=null,this.vtable_in_use=0,this.isNested=!1,this.object_start=0,this.vtables=[],this.vector_num_elems=0,this.force_defaults=!1,this.string_maps=null,this.text_encoder=new TextEncoder;let e;t?e=t:e=1024,this.bb=B.ByteBuffer.allocate(e),this.space=e}clear(){this.bb.clear(),this.space=this.bb.capacity(),this.minalign=1,this.vtable=null,this.vtable_in_use=0,this.isNested=!1,this.object_start=0,this.vtables=[],this.vector_num_elems=0,this.force_defaults=!1,this.string_maps=null}forceDefaults(t){this.force_defaults=t}dataBuffer(){return this.bb}asUint8Array(){return this.bb.bytes().subarray(this.bb.position(),this.bb.position()+this.offset())}prep(t,e){t>this.minalign&&(this.minalign=t);let i=~(this.bb.capacity()-this.space+e)+1&t-1;for(;this.space<i+t+e;){let s=this.bb.capacity();this.bb=l.growByteBuffer(this.bb),this.space+=this.bb.capacity()-s}this.pad(i)}pad(t){for(let e=0;e<t;e++)this.bb.writeInt8(--this.space,0)}writeInt8(t){this.bb.writeInt8(this.space-=1,t)}writeInt16(t){this.bb.writeInt16(this.space-=2,t)}writeInt32(t){this.bb.writeInt32(this.space-=4,t)}writeInt64(t){this.bb.writeInt64(this.space-=8,t)}writeFloat32(t){this.bb.writeFloat32(this.space-=4,t)}writeFloat64(t){this.bb.writeFloat64(this.space-=8,t)}addInt8(t){this.prep(1,0),this.writeInt8(t)}addInt16(t){this.prep(2,0),this.writeInt16(t)}addInt32(t){this.prep(4,0),this.writeInt32(t)}addInt64(t){this.prep(8,0),this.writeInt64(t)}addFloat32(t){this.prep(4,0),this.writeFloat32(t)}addFloat64(t){this.prep(8,0),this.writeFloat64(t)}addFieldInt8(t,e,i){(this.force_defaults||e!=i)&&(this.addInt8(e),this.slot(t))}addFieldInt16(t,e,i){(this.force_defaults||e!=i)&&(this.addInt16(e),this.slot(t))}addFieldInt32(t,e,i){(this.force_defaults||e!=i)&&(this.addInt32(e),this.slot(t))}addFieldInt64(t,e,i){(this.force_defaults||e!==i)&&(this.addInt64(e),this.slot(t))}addFieldFloat32(t,e,i){(this.force_defaults||e!=i)&&(this.addFloat32(e),this.slot(t))}addFieldFloat64(t,e,i){(this.force_defaults||e!=i)&&(this.addFloat64(e),this.slot(t))}addFieldOffset(t,e,i){(this.force_defaults||e!=i)&&(this.addOffset(e),this.slot(t))}addFieldStruct(t,e,i){e!=i&&(this.nested(e),this.slot(t))}nested(t){if(t!=this.offset())throw new TypeError("FlatBuffers: struct must be serialized inline.")}notNested(){if(this.isNested)throw new TypeError("FlatBuffers: object serialization must not be nested.")}slot(t){this.vtable!==null&&(this.vtable[t]=this.offset())}offset(){return this.bb.capacity()-this.space}static growByteBuffer(t){let e=t.capacity();if(e&3221225472)throw new Error("FlatBuffers: cannot grow buffer beyond 2 gigabytes.");let i=e<<1,s=B.ByteBuffer.allocate(i);return s.setPosition(i-e),s.bytes().set(t.bytes(),i-e),s}addOffset(t){this.prep(h.SIZEOF_INT,0),this.writeInt32(this.offset()-t+h.SIZEOF_INT)}startObject(t){this.notNested(),this.vtable==null&&(this.vtable=[]),this.vtable_in_use=t;for(let e=0;e<t;e++)this.vtable[e]=0;this.isNested=!0,this.object_start=this.offset()}endObject(){if(this.vtable==null||!this.isNested)throw new Error("FlatBuffers: endObject called without startObject");this.addInt32(0);let t=this.offset(),e=this.vtable_in_use-1;for(;e>=0&&this.vtable[e]==0;e--);let i=e+1;for(;e>=0;e--)this.addInt16(this.vtable[e]!=0?t-this.vtable[e]:0);let s=2;this.addInt16(t-this.object_start);let a=(i+s)*h.SIZEOF_SHORT;this.addInt16(a);let u=0,j=this.space;t:for(e=0;e<this.vtables.length;e++){let O=this.bb.capacity()-this.vtables[e];if(a==this.bb.readInt16(O)){for(let I=h.SIZEOF_SHORT;I<a;I+=h.SIZEOF_SHORT)if(this.bb.readInt16(j+I)!=this.bb.readInt16(O+I))continue t;u=this.vtables[e];break}}return u?(this.space=this.bb.capacity()-t,this.bb.writeInt32(this.space,u-t)):(this.vtables.push(this.offset()),this.bb.writeInt32(this.bb.capacity()-t,this.offset()-t)),this.isNested=!1,t}finish(t,e,i){let s=i?h.SIZE_PREFIX_LENGTH:0;if(e){let a=e;if(this.prep(this.minalign,h.SIZEOF_INT+h.FILE_IDENTIFIER_LENGTH+s),a.length!=h.FILE_IDENTIFIER_LENGTH)throw new TypeError("FlatBuffers: file identifier must be length "+h.FILE_IDENTIFIER_LENGTH);for(let u=h.FILE_IDENTIFIER_LENGTH-1;u>=0;u--)this.writeInt8(a.charCodeAt(u))}this.prep(this.minalign,h.SIZEOF_INT+s),this.addOffset(t),s&&this.addInt32(this.bb.capacity()-this.space),this.bb.setPosition(this.space)}finishSizePrefixed(t,e){this.finish(t,e,!0)}requiredField(t,e){let i=this.bb.capacity()-t,s=i-this.bb.readInt32(i);if(!(e<this.bb.readInt16(s)&&this.bb.readInt16(s+e)!=0))throw new TypeError("FlatBuffers: field "+e+" must be set")}startVector(t,e,i){this.notNested(),this.vector_num_elems=e,this.prep(h.SIZEOF_INT,t*e),this.prep(i,t*e)}endVector(){return this.writeInt32(this.vector_num_elems),this.offset()}createSharedString(t){if(!t)return 0;if(this.string_maps||(this.string_maps=new Map),this.string_maps.has(t))return this.string_maps.get(t);let e=this.createString(t);return this.string_maps.set(t,e),e}createString(t){if(t==null)return 0;let e;return t instanceof Uint8Array?e=t:e=this.text_encoder.encode(t),this.addInt8(0),this.startVector(1,e.length,1),this.bb.setPosition(this.space-=e.length),this.bb.bytes().set(e,this.space),this.endVector()}createByteVector(t){return t==null?0:(this.startVector(1,t.length,1),this.bb.setPosition(this.space-=t.length),this.bb.bytes().set(t,this.space),this.endVector())}createObjectOffset(t){return t===null?0:typeof t=="string"?this.createString(t):t.pack(this)}createObjectOffsetList(t){let e=[];for(let i=0;i<t.length;++i){let s=t[i];if(s!==null)e.push(this.createObjectOffset(s));else throw new TypeError("FlatBuffers: Argument for createObjectOffsetList cannot contain null.")}return e}createStructOffsetList(t,e){return e(this,t.length),this.createObjectOffsetList(t.slice().reverse()),this.endVector()}};p.Builder=N});var D=b(r=>{Object.defineProperty(r,"__esModule",{value:!0});r.ByteBuffer=r.Builder=r.Encoding=r.isLittleEndian=r.float64=r.float32=r.int32=r.SIZE_PREFIX_LENGTH=r.FILE_IDENTIFIER_LENGTH=r.SIZEOF_INT=r.SIZEOF_SHORT=void 0;var v=_();Object.defineProperty(r,"SIZEOF_SHORT",{enumerable:!0,get:function(){return v.SIZEOF_SHORT}});var R=_();Object.defineProperty(r,"SIZEOF_INT",{enumerable:!0,get:function(){return R.SIZEOF_INT}});var P=_();Object.defineProperty(r,"FILE_IDENTIFIER_LENGTH",{enumerable:!0,get:function(){return P.FILE_IDENTIFIER_LENGTH}});var U=_();Object.defineProperty(r,"SIZE_PREFIX_LENGTH",{enumerable:!0,get:function(){return U.SIZE_PREFIX_LENGTH}});var F=y();Object.defineProperty(r,"int32",{enumerable:!0,get:function(){return F.int32}});Object.defineProperty(r,"float32",{enumerable:!0,get:function(){return F.float32}});Object.defineProperty(r,"float64",{enumerable:!0,get:function(){return F.float64}});Object.defineProperty(r,"isLittleEndian",{enumerable:!0,get:function(){return F.isLittleEndian}});var Z=g();Object.defineProperty(r,"Encoding",{enumerable:!0,get:function(){return Z.Encoding}});var H=L();Object.defineProperty(r,"Builder",{enumerable:!0,get:function(){return H.Builder}});var G=T();Object.defineProperty(r,"ByteBuffer",{enumerable:!0,get:function(){return G.ByteBuffer}})});return D();})();
          
          // somewhat minified version of https://gist.github.com/enepomnyaschih/72c423f727d395eeaa09697058238727 that provides base64ToBytes
          const base64abc=["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","0","1","2","3","4","5","6","7","8","9","+","/"];
          const base64codes=[255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,62,255,255,255,63,52,53,54,55,56,57,58,59,60,61,255,255,255,0,255,255,255,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,255,255,255,255,255,255,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51];
          function getBase64Code(e){if(e>=base64codes.length)throw Error("Unable to parse base64 string.");let t=base64codes[e];if(255===t)throw Error("Unable to parse base64 string.");return t}
          function bytesToBase64(e){let t="",a,s=e.length;for(a=2;a<s;a+=3)t+=base64abc[e[a-2]>>2],t+=base64abc[(3&e[a-2])<<4|e[a-1]>>4],t+=base64abc[(15&e[a-1])<<2|e[a]>>6],t+=base64abc[63&e[a]];return a===s+1&&(t+=base64abc[e[a-2]>>2],t+=base64abc[(3&e[a-2])<<4],t+="=="),a===s&&(t+=base64abc[e[a-2]>>2],t+=base64abc[(3&e[a-2])<<4|e[a-1]>>4],t+=base64abc[(15&e[a-1])<<2],t+="="),t}
          function base64ToBytes(e){if(e.length%4!=0)throw Error("Unable to parse base64 string.");let t=e.indexOf("=");if(-1!==t&&t<e.length-2)throw Error("Unable to parse base64 string.");let a=e.endsWith("==")?2:e.endsWith("=")?1:0,s=e.length,o=new Uint8Array(3*(s/4)),b;for(let n=0,r=0;n<s;n+=4,r+=3)b=getBase64Code(e.charCodeAt(n))<<18|getBase64Code(e.charCodeAt(n+1))<<12|getBase64Code(e.charCodeAt(n+2))<<6|getBase64Code(e.charCodeAt(n+3)),o[r]=b>>16,o[r+1]=b>>8&255,o[r+2]=255&b;return o.subarray(0,o.length-a)}
          function base64encode(e,t=new TextEncoder){return bytesToBase64(t.encode(e))}
          function base64decode(e,t=new TextDecoder){return t.decode(base64ToBytes(e))}
          
          // Flatbuffer generated CurrentWeather class
          class CurrentWeather {
            constructor() {
                this.bb = null;
                this.bb_pos = 0;
            }
            __init(i, bb) {
                this.bb_pos = i;
                this.bb = bb;
                return this;
            }
            static getRootAsCurrentWeather(bb, obj) {
                return (obj || new CurrentWeather()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
            }
            static getSizePrefixedRootAsCurrentWeather(bb, obj) {
                bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
                return (obj || new CurrentWeather()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
            }
            outdoorTemperature() {
                const offset = this.bb.__offset(this.bb_pos, 4);
                return offset ? this.bb.readFloat32(this.bb_pos + offset) : 0.0;
            }
            windDirection() {
                const offset = this.bb.__offset(this.bb_pos, 6);
                return offset ? this.bb.readFloat32(this.bb_pos + offset) : 0.0;
            }
            windSpeed() {
                const offset = this.bb.__offset(this.bb_pos, 8);
                return offset ? this.bb.readFloat32(this.bb_pos + offset) : 0.0;
            }
            static startCurrentWeather(builder) {
                builder.startObject(3);
            }
            static addOutdoorTemperature(builder, outdoorTemperature) {
                builder.addFieldFloat32(0, outdoorTemperature, 0.0);
            }
            static addWindDirection(builder, windDirection) {
                builder.addFieldFloat32(1, windDirection, 0.0);
            }
            static addWindSpeed(builder, windSpeed) {
                builder.addFieldFloat32(2, windSpeed, 0.0);
            }
            static endCurrentWeather(builder) {
                const offset = builder.endObject();
                return offset;
            }
            static createCurrentWeather(builder, outdoorTemperature, windDirection, windSpeed) {
                CurrentWeather.startCurrentWeather(builder);
                CurrentWeather.addOutdoorTemperature(builder, outdoorTemperature);
                CurrentWeather.addWindDirection(builder, windDirection);
                CurrentWeather.addWindSpeed(builder, windSpeed);
                return CurrentWeather.endCurrentWeather(builder);
            }
          }

          // Flatbuffer generated Weather class
          class Weather {
              constructor() {
                  this.bb = null;
                  this.bb_pos = 0;
              }
              __init(i, bb) {
                  this.bb_pos = i;
                  this.bb = bb;
                  return this;
              }
              static getRootAsWeather(bb, obj) {
                  return (obj || new Weather()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
              }
              static getSizePrefixedRootAsWeather(bb, obj) {
                  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
                  return (obj || new Weather()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
              }
              current(obj) {
                  const offset = this.bb.__offset(this.bb_pos, 4);
                  return offset ? (obj || new CurrentWeather()).__init(this.bb.__indirect(this.bb_pos + offset), this.bb) : null;
              }
              static startWeather(builder) {
                  builder.startObject(1);
              }
              static addCurrent(builder, currentOffset) {
                  builder.addFieldOffset(0, currentOffset, 0);
              }
              static endWeather(builder) {
                  const offset = builder.endObject();
                  return offset;
              }
              static finishWeatherBuffer(builder, offset) {
                  builder.finish(offset);
              }
              static finishSizePrefixedWeatherBuffer(builder, offset) {
                  builder.finish(offset, undefined, true);
              }
              static createWeather(builder, currentOffset) {
                  Weather.startWeather(builder);
                  Weather.addCurrent(builder, currentOffset);
                  return Weather.endWeather(builder);
              }
          }
          
          // -----------------------
          //   Actual code
          // -----------------------
          //let data = new Uint8Array();
          //new flatbuffers.ByteBuffer(data);
          
          let builder = new flatbuffers.Builder(1024);
          
          let currentWeather = CurrentWeather.createCurrentWeather(builder, 10.5, 180, 2.4);
          
          let weather = Weather.createWeather(builder, currentWeather);
          builder.finish(weather);
          
          let buf = builder.asUint8Array();
          
          let item = items.FlatBufferContent;
          item.sendCommand(bytesToBase64(buf));

        })

        ();
```